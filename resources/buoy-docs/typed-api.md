---
title: "audiocuekit typed API"
summary: "Source-derived public signatures, properties, parameters, defaults, and return values."
category: "Reference"
order: 21
types: ["AudioCueMixer"]
dependencies: []
symbols: ["AudioCueMixer"]
related: ["api.md", "getting-started.md", "index.md"]
---

# audiocuekit typed API

Generated from the package source declarations. Platform guards are shown where present; inherited members are documented on the base type. See the [API contracts](api.md) for behavior and limitations.

## AudioCueMixer

`Public Class AudioCueMixer`

### Properties

| Name | Type | Read-Only | Shared | Default | Description |
| --- | --- | --- | --- | --- | --- |
| `LastError` | `String` | No | No | `""` | — |
| `Volume` | `Double` | No | No | — | — |

### Constructor

`Public Sub Constructor(output As Boolean = True)`

#### Parameters

| Name | Type | Passing | Default |
| --- | --- | --- | --- |
| `output` | `Boolean` | `ByVal` | `True` |

Returns: no value.

### Load

`Public Function Load(filePath As String) As Integer`

#### Parameters

| Name | Type | Passing | Default |
| --- | --- | --- | --- |
| `filePath` | `String` | `ByVal` | Required |

#### Returns

| Type |
| --- |
| `Integer` |

### Play

`Public Function Play(clip As Integer, gain As Double = 0.65) As Boolean`

#### Parameters

| Name | Type | Passing | Default |
| --- | --- | --- | --- |
| `clip` | `Integer` | `ByVal` | Required |
| `gain` | `Double` | `ByVal` | `0.65` |

#### Returns

| Type |
| --- |
| `Boolean` |

### SetPaused

`Public Sub SetPaused(paused As Boolean)`

#### Parameters

| Name | Type | Passing | Default |
| --- | --- | --- | --- |
| `paused` | `Boolean` | `ByVal` | Required |

Returns: no value.

### Stop

`Public Sub Stop()`

No parameters.

Returns: no value.

### Close

`Public Sub Close()`

No parameters.

Returns: no value.

### Destructor

`Public Sub Destructor()`

No parameters.

Returns: no value.
