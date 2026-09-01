#pragma once

#include "daisy_seed.h"
#include "daisysp.h"
#include "EffectManager.h"

namespace pedal
{
class AudioEngine
{
  public:
    explicit AudioEngine(EffectManager& effect_manager);

    void Init(float sample_rate);
    void ProcessBlock(daisy::AudioHandle::InputBuffer  input,
                      daisy::AudioHandle::OutputBuffer output,
                      size_t                           size);

  private:
    EffectManager&   effect_manager_;
    daisysp::DcBlock dc_block_;
};
} // namespace pedal
