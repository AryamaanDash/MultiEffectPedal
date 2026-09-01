#include "EffectManager.h"

namespace pedal
{
EffectManager::EffectManager(EffectProcessor& bypass,
                             EffectProcessor& distortion,
                             EffectProcessor& reverb,
                             EffectProcessor& delay,
                             EffectProcessor& flanger)
: processors_{&bypass, &distortion, &reverb, &delay, &flanger},
  requested_mode_(static_cast<uint8_t>(EffectMode::OFF)),
  active_mode_(EffectMode::OFF)
{
}

void EffectManager::Init(float sample_rate)
{
    for(size_t index = 0; index < kEffectModeCount; ++index)
    {
        processors_[index]->Init(sample_rate);
    }
}

void EffectManager::RequestMode(EffectMode mode)
{
    const size_t mode_index = EffectModeIndex(mode);
    if(mode_index >= kEffectModeCount)
    {
        mode = EffectMode::OFF;
    }

    requested_mode_.store(static_cast<uint8_t>(mode),
                          std::memory_order_release);
}

EffectMode EffectManager::RequestNextMode()
{
    const uint8_t current = requested_mode_.load(std::memory_order_acquire);
    const uint8_t next
        = static_cast<uint8_t>((current + 1U) % kEffectModeCount);
    const EffectMode next_mode = static_cast<EffectMode>(next);

    RequestMode(next_mode);
    return next_mode;
}

EffectMode EffectManager::RequestedMode() const
{
    return static_cast<EffectMode>(
        requested_mode_.load(std::memory_order_acquire));
}

void EffectManager::ApplyRequestedMode()
{
    active_mode_ = RequestedMode();
}

StereoFrame EffectManager::Process(float input)
{
    const size_t active_index = EffectModeIndex(active_mode_);
    StereoFrame  selected     = MonoFrame(input);

    // Stateful effects still process silence while inactive so delay and
    // reverb tails decay instead of being frozen until the mode is revisited.
    for(size_t index = 0; index < kEffectModeCount; ++index)
    {
        const float processor_input = index == active_index ? input : 0.0f;
        const StereoFrame candidate
            = processors_[index]->Process(processor_input);

        if(index == active_index)
        {
            selected = candidate;
        }
    }

    return selected;
}
} // namespace pedal
