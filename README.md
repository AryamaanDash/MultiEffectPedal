# MultiEffectPedal

## Description

Multi-effect guitar pedal firmware and a custom KiCad PCB for the Daisy Seed.
The firmware runs distortion, stereo reverb, delay, and flanger effects at the
Daisy audio sample rate, with a footswitch for cycling through the effects.

## Firmware architecture

```text
src/main.cpp             Hardware composition and application entry point
src/AudioEngine.cpp      Real-time block processing and output limiting
src/EffectManager.cpp    Effect ownership-independent routing and mode changes
src/PedalControls.cpp    Foreground switch scanning and LED state
src/effects/             Individual DSP effect adapters
include/                 Public interfaces and shared audio types
```

Every effect implements the `EffectProcessor` interface:

```cpp
virtual void Init(float sample_rate) = 0;
virtual void SetParameters(const EffectParameters& parameters) = 0;
virtual StereoFrame Process(float input) = 0;
```

Effect parameters use normalized values from 0 to 1. Each adapter maps those
values to the range expected by its DaisySP processor. Mode changes are sent
from the foreground loop through an atomic request and applied by the audio
engine at the next block boundary.

The reverb and one-second delay buffers remain in external SDRAM. Effect
objects and the manager are statically allocated; audio processing performs no
dynamic allocation.

## Building

This project expects `libDaisy` and `DaisySP` to be available at the paths in
the `Makefile`.

```sh
make
```

To add an effect, implement `EffectProcessor`, register the instance with
`EffectManager`, and add its source file to `CPP_SOURCES` in the `Makefile`.

## Tools

- Daisy Seed and libDaisy
- DaisySP and DaisySP-LGPL
- KiCad PCB and schematic design
