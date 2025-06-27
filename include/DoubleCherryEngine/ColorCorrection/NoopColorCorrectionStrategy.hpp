#pragma once
#include <DoubleCherryEngine/common/interfaces/ISingleton.hpp>
#include "IColorCorrectionStrategy.hpp"

class NoopColorCorrectionStrategy : public ISingleton<NoopColorCorrectionStrategy>, IColorCorrectionStrategy {
    friend class ISingleton<NoopColorCorrectionStrategy>; 
public:
    uint16_t apply(uint16_t color) const override {
        return color; 
    }
};