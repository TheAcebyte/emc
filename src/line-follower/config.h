#pragma once

#include "types.h"

namespace LineFollowerPins {
// Pins
constexpr int ENA = 6;
constexpr int IN1 = 9;
constexpr int IN2 = 10;
constexpr int ENB = 5;
constexpr int IN3 = 7;
constexpr int IN4 = 8;
constexpr int SENSOR_LEFT = 13;
constexpr int SENSOR_MIDDLE = 2;
constexpr int SENSOR_RIGHT = 12;
}; // namespace LineFollowerPins

namespace LineFollowerConfig {
using namespace LineFollowerTypes;
using namespace LineFollowerPins;

// Motors
constexpr int BASE_SPEED = 70;
constexpr int TURN_SPEED = 100;

// Movement mappings
inline constexpr MovementMapping MOVEMENT_MAPPINGS[] = {
    {Movement::FORWARD, {.left = BASE_SPEED, .right = BASE_SPEED}},
    {Movement::LEFT, {.left = -TURN_SPEED, .right = TURN_SPEED}},
    {Movement::RIGHT, {.left = TURN_SPEED, .right = -TURN_SPEED}},
    {Movement::STOP, {.left = 0, .right = 0}},
};

// Sensor mappings
inline constexpr SensorMapping SENSOR_MAPPINGS[] = {
    {{.left = false, .middle = true, .right = false}, Movement::FORWARD},
    {{.left = true, .middle = true, .right = true}, Movement::FORWARD},
    {{.left = true, .middle = true, .right = false}, Movement::LEFT},
    {{.left = false, .middle = true, .right = true}, Movement::RIGHT},
    {{.left = true, .middle = false, .right = false}, Movement::LEFT},
    {{.left = false, .middle = false, .right = true}, Movement::RIGHT},
};
}; // namespace LineFollowerConfig

namespace LineFollowerPIDConfig {
using namespace LineFollowerPins;

// Motors
constexpr int MAX_SPEED = 120;
constexpr int BASE_SPEED = 90;

// PID
constexpr double MAX_INTEGRAL = 255;
constexpr double KE = 1000;
constexpr double KP = 5.0;
constexpr double KI = 0.0;
constexpr double KD = 1.0;
}; // namespace LineFollowerPIDConfig
