#pragma once

#include <cstdint>

class IColorCorrectionStrategy {
public:
    virtual ~IColorCorrectionStrategy() = default;

    virtual uint16_t apply(uint16_t color) const = 0;
};