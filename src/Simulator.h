#pragma once
#include "Vehicle.h"
#include <vector>

class Simulator {
public:
    Simulator();
    void update(float deltaTime);
    const std::vector<Vehicle>& getVehicles() const;
private:
    std::vector<Vehicle> vehicles;
    float observerX = 0.0f;
    float observerY = 0.0f;
    void calculate();
};
