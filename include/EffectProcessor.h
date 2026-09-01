#pragma once

#include "AudioTypes.h"

namespace pedal
{
// All control values are normalized to the range [0, 1]. Concrete effects
// decide how amount and time_rate map to their DSP-specific parameter ranges.
struct EffectParameters
{
    float mix;
    float amount;
    float time_rate;
    float feedback;
};

// Non-owning polymorphic interface. Effect instances are statically allocated;
// they are never created or destroyed through an EffectProcessor pointer.
class EffectProcessor
{
  public:
    virtual void        Init(float sample_rate)                           = 0;
    virtual void        SetParameters(const EffectParameters& parameters) = 0;
    virtual StereoFrame Process(float input)                              = 0;

  protected:
    ~EffectProcessor() = default;
};
} // namespace pedal
