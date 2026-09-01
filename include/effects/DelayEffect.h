#pragma once

#include "daisysp.h"
#include "EffectProcessor.h"

namespace pedal
{
constexpr size_t kMaxDelaySamples = 48000;
using DelayBuffer                 = daisysp::DelayLine<float, kMaxDelaySamples>;

class DelayEffect : public EffectProcessor
{
  public:
    explicit DelayEffect(DelayBuffer& delay_buffer);

    void        Init(float sample_rate) override;
    void        SetParameters(const EffectParameters& parameters) override;
    StereoFrame Process(float input) override;

  private:
    DelayBuffer&     delay_buffer_;
    EffectParameters parameters_;
    float            sample_rate_;
    float            feedback_;
};
} // namespace pedal
