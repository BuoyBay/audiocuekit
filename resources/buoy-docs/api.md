---
title: "AudioCueKit API contracts"
summary: "Mixer lifetime, bounded loading, playback and error handling."
category: "Reference"
symbols: ["AudioCueMixer"]
related: ["index.md", "typed-api.md"]
---

# AudioCueKit API contracts

`AudioCueMixer(output As Boolean = True)` allocates a mixer. Allocation failure
throws `InvalidArgumentException`. `Close` is final and idempotent; destruction
also closes it. Load, Play, SetPaused, Stop and assigning Volume throw
`InvalidArgumentException` after close. Reading Volume returns its retained value.

`Load(filePath)` returns a clip ID, or -1 on failure. Embedded NUL in the path
throws `InvalidArgumentException`. Loaded clips remain until Close; there is no
individual unload operation.

`Play(clip, gain = 0.65)` returns whether a voice was accepted. Clip IDs outside
0..31 and gain outside 0..1, including NaN, throw `InvalidArgumentException`.
An in-range ID that has not been loaded returns False. Device startup failure or
all 16 voices being busy also returns False. Acceptance does not prove audible
output or completion.

`LastError` is refreshed after Load and Play, but not every rejection sets a new
message: clip-capacity exhaustion and an unloaded in-range clip can retain an
older message. Always inspect the return value first.

Volume defaults to 1.0 and changes all active voices. Assigning a value outside
0..1 or NaN throws `InvalidArgumentException`. SetPaused freezes/resumes voice
positions. Stop clears all active voices and retains loaded clips.

Physical audio playback and device hotplug are outside the offline validation
contract. No device access is required to read documentation or load clips.


[Full typed API: properties, parameters, defaults, and returns](typed-api.md)
