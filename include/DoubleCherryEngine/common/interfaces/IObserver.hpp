#pragma once

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void Update() = 0;
};
