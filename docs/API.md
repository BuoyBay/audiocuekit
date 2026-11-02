# AudioCueKit

`Import audiocuekit`. `AudioCueMixer(output=True)` owns up to 32 WAV clips and
16 simultaneous voices. `Load(filePath)` returns a clip ID or -1 with LastError;
`Play(id,gain=0.65)` returns acceptance. Volume accepts 0..1 and changes all live
voices. SetPaused freezes voice positions. Stop clears voices. Close is final
and idempotent. Use from one owning thread; SDL's callback synchronizes internally.

Only 48 kHz mono/stereo signed 16-bit little-endian PCM WAV is accepted, with each
chunk limited to 16 MiB and at most 128 chunks. Importing, construction and loading
are silent. The device opens on the first Play. Missing SDL2 or unavailable audio
hardware returns False with LastError, allowing the application to continue.
The native bridge dynamically uses SDL2's stable public audio ABI; no Bevy or
Rust runtime is used. Linux needs libSDL2-2.0.so.0; macOS needs SDL2 installed
(Homebrew `sdl2` is searched in standard prefixes); Windows needs SDL2.dll on its
native library search path. Physical playback on macOS/Windows needs verification.

`output=False` mixes offline without opening an audio device. Keep mixer calls on one owning thread.

Windows native C builds require a matching Windows SDK and compiler/import-library toolchain. Deploy `SDL2.dll` through the normal native library search path.

The native bridge source is included under `native/`; build it for the architecture of the consuming application.
