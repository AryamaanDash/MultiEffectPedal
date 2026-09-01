#pragma once

#include "EffectProcessor.h"

namespace pedal
{
class BypassEffect : public EffectProcessor
{
  public:
    void        Init(float sample_rate) override;
    void        SetParameters(const EffectParameters& parameters) override;
    StereoFrame Process(float input) override;
};
} // namespace pedal
