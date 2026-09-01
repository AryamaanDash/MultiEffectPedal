#pragma once

#include "daisy_seed.h"
#include "EffectManager.h"

namespace pedal
{
class PedalControls
{
  public:
    PedalControls(daisy::DaisySeed& hardware, EffectManager& effect_manager);

    void Init();
    void Process();

  private:
    daisy::DaisySeed& hardware_;
    EffectManager&    effect_manager_;
    daisy::Switch     mode_switch_;
};
} // namespace pedal
