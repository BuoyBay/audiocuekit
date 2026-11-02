# Mix PCM WAV cues through SDL2

`Import audiocuekit`. `AudioCueMixer(output=True)` owns up to 32 WAV clips and
16 simultaneous voices. `Load(filePath)` returns a clip ID or -1 with LastError;
`Play(id,gain=0.65)` returns acceptance. Volume accepts 0..1 and changes all live
voices. SetPaused freezes voice positions. Stop clears voices. Close is final
and idempotent. Use from one owning thread; SDL's callback synchronizes internally.

Windows builds require a matching native compiler, Windows SDK and developer-shell environment. Deploy both the native bridge and its matching import library where required.

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


## Contributing

[Contributing](CONTRIBUTING.md) · [License](LICENSE)
