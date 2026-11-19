#pragma once
#include <string>

struct Vehicle {
    std::string name;
    float x = 0.0f;
    float y = 0.0f;
    float heading = 0.0f;
    float distance = 0.0f;
    float bearing = 0.0f;
};
