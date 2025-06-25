#pragma once

#include "IColorCorrectionStrategy.hpp"

class NoopStrategy : public IColorCorrectionStrategy {
public:
    uint16_t apply(uint16_t color) const override {
        return color; 
    }
};