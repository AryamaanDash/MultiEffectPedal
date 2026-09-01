#pragma once

#include "daisysp.h"
#include "EffectProcessor.h"

namespace pedal
{
class FlangerEffect : public EffectProcessor
{
  public:
    void        Init(float sample_rate) override;
    void        SetParameters(const EffectParameters& parameters) override;
    StereoFrame Process(float input) override;

  private:
    daisysp::Flanger processor_;
    EffectParameters parameters_;
};
} // namespace pedal
