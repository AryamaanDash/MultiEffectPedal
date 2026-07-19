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
- RV5 is wired as a passive output volume control, so the single potentiometer does something with the current firmware.
- RV1-RV4 are optional future ADC control headers. Do not populate them unless you also update the firmware to read ADC values.

Key nets:
- D9_SWITCH: momentary footswitch to GND
- AUDIO_IN_L: buffered guitar input into Daisy audio input left
- AUDIO_OUT_L: Daisy audio output left into output buffer
- VREF_1V65: half-supply bias for the single-supply op-amp sections
- OUT_LEVEL_IN: AC-coupled signal into RV5 volume lug 3
- OUT_JACK_TIP: RV5 wiper to the output jack tip
- +3V3A: Daisy analog 3.3V rail
- GND: AGND and DGND tied together

Build wiring:
- Put the MCP6002 in an 8-pin DIP socket at U1. Pin 1 is the notched/dot end.
- Wire the input jack tip/sleeve to J1 pad 1/pad 2.
- Wire the output jack tip/sleeve to J2 pad 1/pad 2.
- Wire the momentary footswitch to SW1 pad 1 and pad 2.
- Wire the volume pot to RV5: pad 1 = CCW/GND, pad 2 = wiper/output, pad 3 = CW/input from output coupling capacitor.
- Mount RV1-RV4 only if you add ADC support in firmware later.
