/* Bounded PCM WAV cue mixer. SDL2's stable public audio ABI is loaded optionally.
   No device opens until Play; construction/loading/tests are silent. */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef _WIN32
#include <windows.h>
#define API __declspec(dllexport)
#define OPEN(n) LoadLibraryA(n)
#define SYM(h,n) ((void *)GetProcAddress((HMODULE)(h),n))
#define CLOSE(h) FreeLibrary((HMODULE)(h))
#else
#include <dlfcn.h>
#define API __attribute__((visibility("default")))
#define OPEN(n) dlopen(n,RTLD_NOW|RTLD_LOCAL)
#define SYM(h,n) dlsym(h,n)
#define CLOSE(h) dlclose(h)
#endif
#define RATE 48000
#define CLIPS 32
#define VOICES 16
typedef struct { int freq; uint16_t format; uint8_t channels,silence; uint16_t samples,padding; uint32_t size; void (*callback)(void *,uint8_t *,int); void *userdata; } AudioSpec;
typedef struct { int16_t *data; uint32_t frames; } Clip;
typedef struct { int clip; uint32_t position; double gain; } Voice;
typedef struct {
 void *lib; uint32_t device; int output,paused,initialized; double volume; Clip clips[CLIPS]; Voice voices[VOICES]; int count; char error[160];
 int (*init)(uint32_t); void (*quit)(uint32_t); uint32_t (*open)(const char *,int,const AudioSpec *,AudioSpec *,int); void (*close)(uint32_t); void (*pause)(uint32_t,int); void (*lock)(uint32_t); void (*unlock)(uint32_t);
} Mixer;
static uint32_t u32(const uint8_t *p) { return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24; }
static uint16_t u16(const uint8_t *p) { return (uint16_t)(p[0]|p[1]<<8); }
static void locked(Mixer *m) { if(m->device)m->lock(m->device); }
static void unlocked(Mixer *m) { if(m->device)m->unlock(m->device); }
static void render(void *ctx,uint8_t *bytes,int length) {
 Mixer *m=ctx; memset(bytes,0,(size_t)length); if(m->paused)return;
 int16_t *out=(int16_t *)bytes; int frames=length/4;
 for(int f=0;f<frames;f++) { double l=0,r=0;
  for(int i=0;i<VOICES;i++) { Voice *v=&m->voices[i]; if(v->clip<0)continue; Clip *c=&m->clips[v->clip];
   l+=c->data[v->position*2]*v->gain*m->volume; r+=c->data[v->position*2+1]*v->gain*m->volume;
   if(++v->position>=c->frames)v->clip=-1;
  }
  out[f*2]=(int16_t)fmax(-32768,fmin(32767,l)); out[f*2+1]=(int16_t)fmax(-32768,fmin(32767,r));
 }
}
API void *ack_new(int32_t output) { Mixer *m=calloc(1,sizeof(*m)); if(!m)return NULL; m->output=output!=0; m->volume=1; for(int i=0;i<VOICES;i++)m->voices[i].clip=-1; return m; }
API const char *ack_error(Mixer *m) { return m?m->error:"Audio allocation failed"; }
static int start(Mixer *m) {
 if(m->device||!m->output)return 1;
#ifdef _WIN32
 const char *names[]={"SDL2.dll",NULL};
#elif defined(__APPLE__)
 const char *names[]={"libSDL2-2.0.0.dylib","/opt/homebrew/lib/libSDL2-2.0.0.dylib","/usr/local/lib/libSDL2-2.0.0.dylib","/Library/Frameworks/SDL2.framework/SDL2",NULL};
#else
 const char *names[]={"libSDL2-2.0.so.0","libSDL2.so",NULL};
#endif
 for(int i=0;names[i]&&!m->lib;i++)m->lib=OPEN(names[i]);
 if(!m->lib){snprintf(m->error,sizeof(m->error),"SDL2 audio runtime is unavailable");return 0;}
#define LOAD(field,name) do { *(void **)(&m->field)=SYM(m->lib,name); if(!m->field){snprintf(m->error,sizeof(m->error),"SDL2 audio ABI unavailable");return 0;} }while(0)
 LOAD(init,"SDL_InitSubSystem"); LOAD(quit,"SDL_QuitSubSystem"); LOAD(open,"SDL_OpenAudioDevice"); LOAD(close,"SDL_CloseAudioDevice"); LOAD(pause,"SDL_PauseAudioDevice"); LOAD(lock,"SDL_LockAudioDevice"); LOAD(unlock,"SDL_UnlockAudioDevice");
 if(!m->initialized && m->init(0x10)!=0){snprintf(m->error,sizeof(m->error),"Audio subsystem initialization failed");return 0;}
 m->initialized=1;
 AudioSpec wanted={0}; wanted.freq=RATE; wanted.format=0x8010; wanted.channels=2; wanted.samples=512; wanted.callback=render; wanted.userdata=m;
 m->device=m->open(NULL,0,&wanted,NULL,0); if(!m->device){snprintf(m->error,sizeof(m->error),"Audio output device unavailable");return 0;}
 m->pause(m->device,0); return 1;
}
API int32_t ack_load(Mixer *m,const char *path) {
 if(!m||!path||m->count>=CLIPS)return -1;
 FILE *f=fopen(path,"rb"); if(!f){snprintf(m->error,sizeof(m->error),"Cannot read audio cue");return -1;}
 uint8_t h[12]; uint8_t *pcm=NULL; uint32_t size=0; int format=0,channels=0;
 if(fread(h,1,12,f)!=12||memcmp(h,"RIFF",4)||memcmp(h+8,"WAVE",4))goto invalid;
 for(int chunks=0;chunks<128;chunks++) { uint8_t c[8]; if(fread(c,1,8,f)!=8)break; uint32_t n=u32(c+4); if(n>16*1024*1024)goto invalid;
  if(!memcmp(c,"fmt ",4)) { uint8_t spec[16]; if(n<16||fread(spec,1,16,f)!=16)goto invalid; channels=u16(spec+2); format=u16(spec)==1&&(channels==1||channels==2)&&u32(spec+4)==RATE&&u16(spec+14)==16&&u16(spec+12)==channels*2; if(fseek(f,(long)(n-16+(n&1)),SEEK_CUR))goto invalid; }
  else if(!memcmp(c,"data",4)) { if(pcm||n==0||n%2)goto invalid; pcm=malloc(n); if(!pcm||fread(pcm,1,n,f)!=n)goto invalid; size=n; if(n&1)fseek(f,1,SEEK_CUR); }
  else if(fseek(f,(long)(n+(n&1)),SEEK_CUR))goto invalid;
 }
 if(!format||!pcm||size%(channels*2))goto invalid;
 if(channels==1){
  int16_t *stereo=malloc((size_t)size*2);if(!stereo)goto invalid;
  for(uint32_t i=0;i<size/2;i++)stereo[i*2]=stereo[i*2+1]=((int16_t *)pcm)[i];
  free(pcm);pcm=(uint8_t *)stereo;size*=2;
 }
 fclose(f); locked(m); int id=m->count++; m->clips[id].data=(int16_t *)pcm; m->clips[id].frames=size/4; unlocked(m); m->error[0]=0; return id;
invalid: fclose(f); free(pcm); snprintf(m->error,sizeof(m->error),"Cue requires bounded 48kHz mono/stereo PCM16 WAV"); return -1;
}
API int32_t ack_play(Mixer *m,int32_t clip,double gain) {
 if(!m||clip<0||clip>=m->count||!isfinite(gain)||gain<0||gain>1)return 0;
 if(!start(m))return 0;
 locked(m); int slot=-1; for(int i=0;i<VOICES;i++)if(m->voices[i].clip<0){slot=i;break;}
 if(slot>=0)m->voices[slot]=(Voice){clip,0,gain};
 unlocked(m); if(slot>=0)m->error[0]=0;else snprintf(m->error,sizeof(m->error),"All audio voices are busy"); return slot>=0;
}
API void ack_volume(Mixer *m,double value) { if(!m||!isfinite(value)||value<0||value>1)return; locked(m);m->volume=value;unlocked(m); }
API void ack_pause(Mixer *m,int32_t value) { if(!m)return;locked(m);m->paused=value!=0;unlocked(m); }
API void ack_stop(Mixer *m) { if(!m)return;locked(m);for(int i=0;i<VOICES;i++)m->voices[i].clip=-1;unlocked(m); }
/* Offline deterministic render for inaudible tests. Never access concurrently. */
API int32_t ack_render(Mixer *m,int16_t *out,int32_t frames) { if(!m||m->output||!out||frames<0||frames>RATE*10)return 0;render(m,(uint8_t *)out,frames*4);return 1; }
API void ack_free(Mixer *m) { if(!m)return;if(m->device)m->close(m->device);for(int i=0;i<m->count;i++)free(m->clips[i].data);if(m->lib){if(m->initialized&&m->quit)m->quit(0x10);CLOSE(m->lib);}free(m); }
