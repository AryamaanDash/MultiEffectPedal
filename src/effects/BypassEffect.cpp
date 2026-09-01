#include "effects/BypassEffect.h"

namespace pedal
{
void BypassEffect::Init(float sample_rate)
{
    (void)sample_rate;
}

void BypassEffect::SetParameters(const EffectParameters& parameters)
{
    (void)parameters;
}

StereoFrame BypassEffect::Process(float input)
{
    return MonoFrame(input);
}
} // namespace pedal
