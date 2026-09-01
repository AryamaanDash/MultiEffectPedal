#pragma once

#include "daisysp.h"
#include "EffectProcessor.h"

namespace pedal
{
class DistortionEffect : public EffectProcessor
{
  public:
    void        Init(float sample_rate) override;
    void        SetParameters(const EffectParameters& parameters) override;
    StereoFrame Process(float input) override;

  private:
    daisysp::Overdrive processor_;
    EffectParameters   parameters_;
};
} // namespace pedal
