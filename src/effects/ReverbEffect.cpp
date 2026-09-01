#include "effects/ReverbEffect.h"

namespace pedal
{
namespace
{
    constexpr float kMinimumLowpassHz = 500.0f;
    constexpr float kMaximumLowpassHz = 18000.0f;
    constexpr float kDefaultLowpassHz = 10000.0f;
    constexpr float kMaximumFeedback  = 0.99f;

    constexpr EffectParameters kDefaultParameters{
        0.40f,
        (kDefaultLowpassHz - kMinimumLowpassHz)
            / (kMaximumLowpassHz - kMinimumLowpassHz),
        0.0f,
        0.86f,
    };
} // namespace

ReverbEffect::ReverbEffect(daisysp::ReverbSc& processor)
: processor_(processor), parameters_(kDefaultParameters), sample_rate_(48000.0f)
{
}

void ReverbEffect::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    processor_.Init(sample_rate_);
    SetParameters(kDefaultParameters);
}

void ReverbEffect::SetParameters(const EffectParameters& parameters)
{
    parameters_.mix       = ClampNormalized(parameters.mix);
    parameters_.amount    = ClampNormalized(parameters.amount);
    parameters_.time_rate = ClampNormalized(parameters.time_rate);
    parameters_.feedback  = ClampNormalized(parameters.feedback);

    const float requested_lowpass
        = kMinimumLowpassHz
          + parameters_.amount * (kMaximumLowpassHz - kMinimumLowpassHz);
    const float nyquist_safe_lowpass
        = Clamp(requested_lowpass, 0.0f, sample_rate_ * 0.5f);

    processor_.SetLpFreq(nyquist_safe_lowpass);
    processor_.SetFeedback(Clamp(parameters_.feedback, 0.0f, kMaximumFeedback));
}

StereoFrame ReverbEffect::Process(float input)
{
    float wet_left  = 0.0f;
    float wet_right = 0.0f;
    processor_.Process(input, input, &wet_left, &wet_right);

    return {DryWet(input, wet_left, parameters_.mix),
            DryWet(input, wet_right, parameters_.mix)};
}
} // namespace pedal
