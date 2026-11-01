# Mix PCM WAV cues and play them through SDL2

`Import audiocuekit`. `AudioCueMixer(output=True)` owns up to 32 WAV clips and
16 simultaneous voices. `Load(filePath)` returns a clip ID or -1 with LastError;
`Play(id,gain=0.65)` returns acceptance. Volume accepts 0..1 and changes all live
voices. SetPaused freezes voice positions. Stop clears voices. Close is final
and idempotent. Use from one owning thread; SDL's callback synchronizes internally.

Windows C bridge builds select `clang-cl` or `cl` in a Visual Studio developer
shell, falling back to MinGW GCC. Set `CC` or pass `--cc` to select a compiler.
MSVC-style builds produce both the DLL and its `.lib` import library. Native
Windows C compilation requires the Windows SDK and developer-shell environment;
the test VM has neither, so this path has command-contract tests only.

## Install

From a Buoy consumer project with BPM installed:

```sh
bpm add audiocuekit --git https://github.com/BuoyBay/audiocuekit.git --rev refs/heads/main
```

BPM records the resolved commit in its lockfile. Keep that lockfile with the
consumer project for reproducible installs.

## API entrypoints

- `Import audiocuekit`
- `AudioCueMixer(output=True)`
- `Load(filePath)`
- `Play(id,gain=0.65)`
- `CC`

## Build and API reference

Source is in `modules/`, with native/runtime inputs where required. Use the
[package guide](docs/API.md) for backend-specific build prerequisites, ownership
rules and supported targets. Rebuilding a native backend requires that target's
compiler and SDK; sealing an archive alone does not rebuild the backend.

## Local Buoy setup

On your own machine, install Buoy and run:

```sh
buoy ai init --both
```

The generated agent files and editor settings stay local and are ignored by Git.

## Contributing

[Contributing](CONTRIBUTING.md) · [License](LICENSE)
