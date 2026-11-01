---
title: "AudioCueKit"
summary: "Bounded PCM WAV cue mixing with optional SDL2 audio output."
category: "Guide"
symbols: ["AudioCueMixer"]
related: ["api.md", "typed-api.md"]
---

# AudioCueKit

Import `audiocuekit` to use `AudioCueMixer`. One owning thread controls the mixer;
the native audio callback synchronizes internally. Keep the mixer alive while
playing cues, then call `Close` to release clips and the output device.

Construction and loading do not open an audio device. The first accepted playback
attempt opens SDL2 output when output is enabled. Construct with `False` for an
offline mixer that never opens a device. SDL2 must be available for device output.

Load accepts 48 kHz mono or stereo PCM WAV with signed 16-bit little-endian
samples. The mixer holds at most 32 clips and 16 simultaneous voices. WAV parsing
examines at most 128 chunks, with each chunk limited to 16 MiB.

See [API contracts](api.md) and [typed API](typed-api.md).
