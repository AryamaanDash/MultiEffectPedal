#include "PedalControls.h"

namespace pedal
{
PedalControls::PedalControls(daisy::DaisySeed& hardware,
                             EffectManager&    effect_manager)
: hardware_(hardware), effect_manager_(effect_manager)
{
}

void PedalControls::Init()
{
    mode_switch_.Init(daisy::seed::D9);
    hardware_.SetLed(false);
}

void PedalControls::Process()
{
    mode_switch_.Debounce();
    if(mode_switch_.RisingEdge())
    {
        const EffectMode mode = effect_manager_.RequestNextMode();
        hardware_.SetLed(mode != EffectMode::OFF);
    }
}
} // namespace pedal
