#pragma once

namespace LiftingArmConfig {
// Pins
constexpr int RX = 2;
constexpr int TX = 3;
constexpr int ENA = 6;
constexpr int IN1 = 7;
constexpr int IN2 = 8;
constexpr int ENB = 5;
constexpr int IN3 = 4;
constexpr int IN4 = 12;
constexpr int ARM_SERVO = 10;
constexpr int CLAW_SERVO = 9;

// Motors
constexpr int MAX_SPEED = 100;

// Arm servo
constexpr int ARM_MIN_ANGLE = 0;
constexpr int ARM_MAX_ANGLE = 90;
constexpr int ARM_STEP_ANGLE = 5;

// Claw servo
constexpr int CLAW_MIN_ANGLE = 0;
constexpr int CLAW_MAX_ANGLE = 90;
constexpr int CLAW_STEP_ANGLE = 5;

// Framerate
constexpr int TICK_DELAY = 20;
}; // namespace LiftingArmConfig
