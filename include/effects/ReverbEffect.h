#pragma once

#include "daisysp-lgpl.h"
#include "EffectProcessor.h"

namespace pedal
{
class ReverbEffect : public EffectProcessor
{
  public:
    explicit ReverbEffect(daisysp::ReverbSc& processor);

    void        Init(float sample_rate) override;
    void        SetParameters(const EffectParameters& parameters) override;
    StereoFrame Process(float input) override;

  private:
    daisysp::ReverbSc& processor_;
    EffectParameters   parameters_;
    float              sample_rate_;
};
} // namespace pedal
