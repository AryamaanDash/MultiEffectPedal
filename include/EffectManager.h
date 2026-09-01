#pragma once

#include <atomic>

#include "EffectMode.h"
#include "EffectProcessor.h"

namespace pedal
{
class EffectManager
{
  public:
    EffectManager(EffectProcessor& bypass,
                  EffectProcessor& distortion,
                  EffectProcessor& reverb,
                  EffectProcessor& delay,
                  EffectProcessor& flanger);

    void Init(float sample_rate);

    // Called from the foreground/control loop. The audio thread applies the
    // request once at the next block boundary.
    void       RequestMode(EffectMode mode);
    EffectMode RequestNextMode();
    EffectMode RequestedMode() const;

    // Called by AudioEngine only.
    void        ApplyRequestedMode();
    StereoFrame Process(float input);

  private:
    EffectProcessor*     processors_[kEffectModeCount];
    std::atomic<uint8_t> requested_mode_;
    EffectMode           active_mode_;
};
} // namespace pedal
