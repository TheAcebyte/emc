#pragma once

namespace MazeSolverConfig {
// Pins
constexpr int ENA = 6;
constexpr int IN1 = 9;
constexpr int IN2 = 10;
constexpr int ENB = 5;
constexpr int IN3 = 7;
constexpr int IN4 = 8;
constexpr int SERVO = 2;
constexpr int TRIG = 3;
constexpr int ECHO = 4;

// Motors
constexpr int MAX_SPEED = 255;
constexpr int BASE_SPEED = 80;
constexpr int TURN_SPEED = 120;
constexpr int MOVE_DELAY = 500;
constexpr int TURN_DELAY = 500;

// Supersonic sensor
constexpr int SERVO_DELAY = 200;
constexpr int ECHO_TIMEOUT = 3000;
constexpr double SOUND_SPEED = 0.034;
constexpr double WALL_THRESHOLD = 20.0;
}; // namespace MazeSolverConfig
