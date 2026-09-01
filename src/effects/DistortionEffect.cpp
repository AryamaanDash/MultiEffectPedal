#include "effects/DistortionEffect.h"

namespace pedal
{
namespace
{
    constexpr float            kOutputLevel = 0.85f;
    constexpr EffectParameters kDefaultParameters{1.0f, 0.75f, 0.0f, 0.0f};
} // namespace

void DistortionEffect::Init(float sample_rate)
{
    (void)sample_rate;
    processor_.Init();
    SetParameters(kDefaultParameters);
}

void DistortionEffect::SetParameters(const EffectParameters& parameters)
{
    parameters_.mix       = ClampNormalized(parameters.mix);
    parameters_.amount    = ClampNormalized(parameters.amount);
    parameters_.time_rate = ClampNormalized(parameters.time_rate);
    parameters_.feedback  = ClampNormalized(parameters.feedback);

    processor_.SetDrive(parameters_.amount);
}

StereoFrame DistortionEffect::Process(float input)
{
    const float distorted = processor_.Process(input) * kOutputLevel;
    return MonoFrame(DryWet(input, distorted, parameters_.mix));
}
} // namespace pedal
