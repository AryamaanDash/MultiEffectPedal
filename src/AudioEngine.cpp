#include "AudioEngine.h"

namespace pedal
{
AudioEngine::AudioEngine(EffectManager& effect_manager)
: effect_manager_(effect_manager)
{
}

void AudioEngine::Init(float sample_rate)
{
    dc_block_.Init(sample_rate);
    effect_manager_.Init(sample_rate);
}

void AudioEngine::ProcessBlock(daisy::AudioHandle::InputBuffer  input,
                               daisy::AudioHandle::OutputBuffer output,
                               size_t                           size)
{
    effect_manager_.ApplyRequestedMode();

    for(size_t index = 0; index < size; ++index)
    {
        const float       dry   = dc_block_.Process(input[0][index]);
        const StereoFrame frame = effect_manager_.Process(dry);

        output[0][index] = ClampAudio(frame.left);
        output[1][index] = ClampAudio(frame.right);
    }
}
} // namespace pedal
