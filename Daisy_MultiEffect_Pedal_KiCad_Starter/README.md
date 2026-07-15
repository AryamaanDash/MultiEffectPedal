# Daisy Seed Multi-Effect Guitar Pedal KiCad Starter

Files:
- Daisy_MultiEffect_Pedal.kicad_sch
- Daisy_MultiEffect_Pedal.kicad_pcb
- Daisy_MultiEffect_Pedal.kicad_pro

Important:
- This is a starter schematic/PCB template, not a production-ready board.
- Open in KiCad, verify every symbol, footprint, and pinout.
- Run ERC and DRC before fabrication.
- USB power is assumed for the prototype.
- Do not connect 9V, VIN, or 5V to Daisy ADC pins or +3V3A.
- Your current C++ writes mono to both output channels but this PCB routes only AUDIO OUT L to the mono output jack.
- The potentiometers are placed as optional control inputs, but your current firmware does not read ADC values yet.

Key nets:
- D9_SWITCH: momentary footswitch to GND
- AUDIO_IN_L: buffered guitar input into Daisy audio input left
- AUDIO_OUT_L: Daisy audio output left into output buffer
- VREF_1V65: half-supply bias for the single-supply op-amp sections
- +3V3A: Daisy analog 3.3V rail
- GND: AGND and DGND tied together
