#pragma once

namespace MazeSolverConfig {
// Pins
constexpr int ENA = 6;
constexpr int IN1 = 2;
constexpr int IN2 = 4;
constexpr int ENB = 5;
constexpr int IN3 = 7;
constexpr int IN4 = 8;
constexpr int SERVO = 9;
constexpr int TRIG = 3;
constexpr int ECHO = 10;

// Motors
constexpr int ERROR_SCALE = 30;
constexpr int MAX_SPEED = 255;
constexpr int BASE_SPEED = 100;
constexpr int MOVE_DELAY = 350;
constexpr int TURN_SPEED = 120;
constexpr int TURN_DELAY = 200;
constexpr int TURN_THRESHOLD = 2;

// Supersonic sensor
constexpr int SERVO_DELAY = 400;
constexpr int ECHO_TIMEOUT = 30000;
constexpr double SOUND_SPEED = 0.034;
constexpr double FRONT_WALL_THRESHOLD = 20.0;
constexpr double SIDE_WALL_THRESHOLD = 30.0;
constexpr double MAX_DISTANCE = 1000000.0;
}; // namespace MazeSolverConfig
