#include "Simulator.h"
#include <cmath>

namespace {
constexpr float PI = 3.14159265358979323846f;
float normalizeDegrees(float value) {
    value = std::fmod(value, 360.0f);
    return value < 0.0f ? value + 360.0f : value;
}
}

Simulator::Simulator() {
    vehicles = {
        {"T-80BVM", 420.0f, 300.0f, 45.0f},
        {"Leopard 2A6", 680.0f, 100.0f, 90.0f},
        {"M1A2 SEP", -500.0f, -600.0f, 225.0f},
        {"Type 10", 900.0f, 700.0f, 180.0f}
    };
    calculate();
}

void Simulator::update(float deltaTime) {
    for (auto& v : vehicles) {
        const float radians = v.heading * PI / 180.0f;
        v.x += std::sin(radians) * deltaTime * 2.0f;
        v.y += std::cos(radians) * deltaTime * 2.0f;
        v.heading = normalizeDegrees(v.heading + deltaTime * 2.0f);
    }
    calculate();
}

void Simulator::calculate() {
    for (auto& v : vehicles) {
        const float dx = v.x - observerX;
        const float dy = v.y - observerY;
        v.distance = std::sqrt(dx * dx + dy * dy);
        v.bearing = normalizeDegrees(std::atan2(dx, dy) * 180.0f / PI);
    }
}

const std::vector<Vehicle>& Simulator::getVehicles() const {
    return vehicles;
}
