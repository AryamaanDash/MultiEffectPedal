#include "daisy_seed.h"
#include "daisysp.h"
#include "daisysp-lgpl.h"

using namespace daisy;
using namespace daisysp;

// Hardware

DaisySeed hardware;
Switch    mode_switch;

// Effect modes

enum class EffectMode : uint8_t
{
    OFF = 0,
    DISTORTION,
    REVERB,
    DELAY,
    FLANGER,
    COUNT
};

volatile EffectMode current_mode = EffectMode::OFF;

// Effects

Overdrive distortion;
Flanger   flanger;
DcBlock   dc_block;

ReverbSc DSY_SDRAM_BSS reverb;

// One second maximum delay at 48 kHz.
constexpr size_t MAX_DELAY_SAMPLES = 48000;

DelayLine<float, MAX_DELAY_SAMPLES> DSY_SDRAM_BSS delay_line;

//Distortion
constexpr float DISTORTION_DRIVE = 0.75f;
constexpr float DISTORTION_LEVEL = 0.85f;

//Reverb
constexpr float REVERB_FEEDBACK = 0.86f;
constexpr float REVERB_LP_FREQ = 10000.0f;
constexpr float REVERB_WET = 0.40f;

//Delay
constexpr float DELAY_TIME_MS = 375.0f;
constexpr float DELAY_FEEDBACK = 0.45f;
constexpr float DELAY_WET = 0.45f;

//Flanger
constexpr float FLANGER_DELAY_MS = 2.0f;
constexpr float FLANGER_FEEDBACK = 0.45f;
constexpr float FLANGER_DEPTH = 0.80f;
constexpr float FLANGER_RATE_HZ = 0.25f;
constexpr float FLANGER_WET = 0.75f;

//Utility Functions

float ClampAudio(float sample){
	if(sample > 1.0f){
		return 1.0f;
	}

	if(sample < -1.0f){
		return -1.0f;
	}

	return sample;
}

float DryWet(float dry, float wet, float wet_amount){
	return dry * (1.0f - wet_amount) + wet * wet_amount;
}

//Audio Callback

void AudioCallback(AudioHandle::InputBuffer inp, AudioHandle::OutputBuffer out, size_t size){
	const EffectMode mode = current_mode;
	for(size_t i = 0; i < size; ++i){
		float dry = inp[0][i];
		dry = dc_block.Process(dry);

		const float distorted = distortion.Process(dry) * DISTORTION_LEVEL;
		
		float reverb_left = 0.0f;
		float reverb_right = 0.0f;
		
		const float reverb_input = mode == EffectMode::REVERB ? dry : 0.0f;

		reverb.Process(reverb_input, reverb_input, &reverb_left, &reverb_right);

		const float delayed = delay_line.Read();

		const float delay_input = mode == EffectMode::DELAY ? dry : 0.0f;
		delay_line.Write(delay_input + delayed * DELAY_FEEDBACK);

		const float flanger_input = mode == EffectMode::FLANGER ? dry : 0.0f;

		const float flanged = flanger.Process(flanger_input);

		float left_output = dry;
		float right_output = dry;

		switch(mode)
		{
			case EffectMode::OFF:
				left_output = dry;
				right_output = dry;
				break;
			
			case EffectMode::DISTORTION:
				left_output = distorted;
				right_output = distorted;
				break;

			case EffectMode::REVERB:
				left_output = DryWet(dry, reverb_left, REVERB_WET);
				right_output = DryWet(dry, reverb_right, REVERB_WET);
				break;
			
			case EffectMode::DELAY:
				{
					const float delay_output = DryWet(dry, delayed, DELAY_WET);
					left_output = delay_output;
					right_output = delay_output;
					break;
				}

			case EffectMode::FLANGER:
			{
				const float flanger_output = DryWet(dry, flanged, FLANGER_WET);
				left_output = flanger_output;
				right_output = flanger_output;
				break;
			}

			default:
				left_output = dry;
				right_output = dry;
				break;
		}
		out[1][i] = ClampAudio(right_output);
		out[0][i] = ClampAudio(left_output);
	}
}

// Mode Switching
void AdvanceMode(){
	uint8_t next_mode = static_cast<uint8_t>(current_mode) + 1;

	if(next_mode >= static_cast<uint8_t>(EffectMode::COUNT)){
		next_mode = 0;
	}

	current_mode = static_cast<EffectMode>(next_mode);
	
	hardware.SetLed(current_mode != EffectMode::OFF);
}

int main(void){
	hardware.Init();
	hardware.SetAudioBlockSize(4);

	const float sample_rate = hardware.AudioSampleRate();

	mode_switch.Init(seed::D9);
	dc_block.Init(sample_rate);

	distortion.Init();
	distortion.SetDrive(DISTORTION_DRIVE);

	reverb.Init(sample_rate);
	reverb.SetFeedback(REVERB_FEEDBACK);
	reverb.SetLpFreq(REVERB_LP_FREQ);

	delay_line.Init();
	const float delay_samples = sample_rate * DELAY_TIME_MS / 1000.0f;

	delay_line.SetDelay(delay_samples);

	flanger.Init(sample_rate);
	flanger.SetDelayMs(FLANGER_DELAY_MS);
    flanger.SetFeedback(FLANGER_FEEDBACK);
    flanger.SetLfoDepth(FLANGER_DEPTH);
    flanger.SetLfoFreq(FLANGER_RATE_HZ);

	hardware.SetLed(false);
	hardware.StartAudio(AudioCallback);

	while(true){
		mode_switch.Debounce();
		if(mode_switch.RisingEdge()){
			AdvanceMode();
		}

		System::Delay(1);
	}
}