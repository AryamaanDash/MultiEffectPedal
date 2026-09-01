#pragma once

#include <cstddef>
#include <cstdint>

namespace pedal
{
enum class EffectMode : uint8_t
{
    OFF = 0,
    DISTORTION,
    REVERB,
    DELAY,
    FLANGER,
    COUNT
};

constexpr size_t kEffectModeCount = static_cast<size_t>(EffectMode::COUNT);

inline size_t EffectModeIndex(EffectMode mode)
{
    return static_cast<size_t>(mode);
}
} // namespace pedal
