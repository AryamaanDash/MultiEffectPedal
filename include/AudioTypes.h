#pragma once

namespace pedal
{
struct StereoFrame
{
    float left;
    float right;
};

inline StereoFrame MonoFrame(float sample)
{
    return {sample, sample};
}

inline float Clamp(float value, float minimum, float maximum)
{
    if(value < minimum)
    {
        return minimum;
    }

    if(value > maximum)
    {
        return maximum;
    }

    return value;
}

inline float ClampAudio(float sample)
{
    return Clamp(sample, -1.0f, 1.0f);
}

inline float ClampNormalized(float value)
{
    return Clamp(value, 0.0f, 1.0f);
}

inline float DryWet(float dry, float wet, float wet_amount)
{
    const float mix = ClampNormalized(wet_amount);
    return dry * (1.0f - mix) + wet * mix;
}
} // namespace pedal
