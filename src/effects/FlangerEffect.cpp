#include "effects/FlangerEffect.h"

namespace pedal
{
namespace
{
    constexpr float kDelayMs         = 2.0f;
    constexpr float kMinimumRateHz   = 0.05f;
    constexpr float kMaximumRateHz   = 5.0f;
    constexpr float kDefaultRateHz   = 0.25f;
    constexpr float kMaximumFeedback = 0.95f;

    constexpr EffectParameters kDefaultParameters{
        0.75f,
        0.80f,
        (kDefaultRateHz - kMinimumRateHz) / (kMaximumRateHz - kMinimumRateHz),
        0.45f,
    };
} // namespace

void FlangerEffect::Init(float sample_rate)
{
    processor_.Init(sample_rate);
    processor_.SetDelayMs(kDelayMs);
    SetParameters(kDefaultParameters);
}

void FlangerEffect::SetParameters(const EffectParameters& parameters)
{
    parameters_.mix       = ClampNormalized(parameters.mix);
    parameters_.amount    = ClampNormalized(parameters.amount);
    parameters_.time_rate = ClampNormalized(parameters.time_rate);
    parameters_.feedback  = ClampNormalized(parameters.feedback);

    const float rate_hz
        = kMinimumRateHz
          + parameters_.time_rate * (kMaximumRateHz - kMinimumRateHz);

    processor_.SetLfoDepth(parameters_.amount);
    processor_.SetLfoFreq(rate_hz);
    processor_.SetFeedback(Clamp(parameters_.feedback, 0.0f, kMaximumFeedback));
}

StereoFrame FlangerEffect::Process(float input)
{
    const float flanged = processor_.Process(input);
    return MonoFrame(DryWet(input, flanged, parameters_.mix));
}
} // namespace pedal
