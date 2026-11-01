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

`output=False` uses an offline mixer without opening a device. Native test-only
ack_render exposes deterministic PCM for inaudible assertions. Do not call it
concurrently. Python verifier checks asset metadata, silence, volume, pause,
voice exhaustion, malformed WAV rejection and SDL's dummy audio device.

Build: `python3 tools/build_audiocuekit.py`. The builder exposes build(runtime_root)
and seal(target, nativeLibrary, archive=...) for isolated application builders.

Maintainers run the private validation suite before release. See the repository README for public build instructions.

Windows C bridge builds select `clang-cl` or `cl` in a Visual Studio developer
shell, falling back to MinGW GCC. Set `CC` or pass `--cc` to select a compiler.
MSVC-style builds produce both the DLL and its `.lib` import library. Native
Windows C compilation requires the Windows SDK and developer-shell environment;
the test VM has neither, so this path has command-contract tests only.

For a Linux MinGW cross slice:

```sh
python3 tools/build_audiocuekit.py --target x86_64-windows --cc x86_64-w64-mingw32-gcc
```

Maintainers run the private validation suite before release. See the repository README for public build instructions.
