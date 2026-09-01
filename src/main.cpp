#include "daisy_seed.h"
#include "daisysp-lgpl.h"

#include "AudioEngine.h"
#include "EffectManager.h"
#include "PedalControls.h"
#include "effects/BypassEffect.h"
#include "effects/DelayEffect.h"
#include "effects/DistortionEffect.h"
#include "effects/FlangerEffect.h"
#include "effects/ReverbEffect.h"

namespace
{
daisy::DaisySeed hardware;

// These two processors contain large internal buffers and must remain in the
// Daisy Seed's external SDRAM.
daisysp::ReverbSc DSY_SDRAM_BSS  reverb_processor;
pedal::DelayBuffer DSY_SDRAM_BSS delay_buffer;

pedal::BypassEffect     bypass_effect;
pedal::DistortionEffect distortion_effect;
pedal::ReverbEffect     reverb_effect(reverb_processor);
pedal::DelayEffect      delay_effect(delay_buffer);
pedal::FlangerEffect    flanger_effect;

pedal::EffectManager effect_manager(bypass_effect,
                                    distortion_effect,
                                    reverb_effect,
                                    delay_effect,
                                    flanger_effect);
pedal::AudioEngine   audio_engine(effect_manager);
pedal::PedalControls controls(hardware, effect_manager);

void AudioCallback(daisy::AudioHandle::InputBuffer  input,
                   daisy::AudioHandle::OutputBuffer output,
                   size_t                           size)
{
    audio_engine.ProcessBlock(input, output, size);
}
} // namespace

int main(void)
{
    hardware.Init();
    hardware.SetAudioBlockSize(4);

    audio_engine.Init(hardware.AudioSampleRate());
    controls.Init();

    hardware.StartAudio(AudioCallback);

    while(true)
    {
        controls.Process();
        daisy::System::Delay(1);
    }
}
