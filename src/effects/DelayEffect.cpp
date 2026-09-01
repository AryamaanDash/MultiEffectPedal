#include "effects/DelayEffect.h"

namespace pedal
{
namespace
{
    constexpr float kMinimumDelayMs  = 20.0f;
    constexpr float kMaximumDelayMs  = 1000.0f;
    constexpr float kDefaultDelayMs  = 375.0f;
    constexpr float kMaximumFeedback = 0.95f;

    constexpr EffectParameters kDefaultParameters{
        0.45f,
        0.0f,
        (kDefaultDelayMs - kMinimumDelayMs)
            / (kMaximumDelayMs - kMinimumDelayMs),
        0.45f,
    };
} // namespace

DelayEffect::DelayEffect(DelayBuffer& delay_buffer)
: delay_buffer_(delay_buffer),
  parameters_(kDefaultParameters),
  sample_rate_(48000.0f),
  feedback_(kDefaultParameters.feedback)
{
}

void DelayEffect::Init(float sample_rate)
{
    sample_rate_ = sample_rate;
    delay_buffer_.Init();
    SetParameters(kDefaultParameters);
}

void DelayEffect::SetParameters(const EffectParameters& parameters)
{
    parameters_.mix       = ClampNormalized(parameters.mix);
    parameters_.amount    = ClampNormalized(parameters.amount);
    parameters_.time_rate = ClampNormalized(parameters.time_rate);
    parameters_.feedback  = ClampNormalized(parameters.feedback);

    const float delay_ms
        = kMinimumDelayMs
          + parameters_.time_rate * (kMaximumDelayMs - kMinimumDelayMs);
    delay_buffer_.SetDelay(sample_rate_ * delay_ms / 1000.0f);
    feedback_ = Clamp(parameters_.feedback, 0.0f, kMaximumFeedback);
}

StereoFrame DelayEffect::Process(float input)
{
    const float delayed = delay_buffer_.Read();
    delay_buffer_.Write(input + delayed * feedback_);

    return MonoFrame(DryWet(input, delayed, parameters_.mix));
}
} // namespace pedal
