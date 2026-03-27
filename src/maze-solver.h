#pragma once

#include "task.h"
#include "utils.h"
#include <Arduino.h>
#include <Servo.h>

struct WallState {
  bool left;
  bool front;
  bool right;
};

class MazeSolver : Task {
private:
  static constexpr int ENA = 6;
  static constexpr int IN1 = 9;
  static constexpr int IN2 = 10;

  static constexpr int ENB = 5;
  static constexpr int IN3 = 7;
  static constexpr int IN4 = 8;

  static constexpr int SERVO = 2;
  static constexpr int TRIG = 3;
  static constexpr int ECHO = 4;

  static constexpr int SERVO_DELAY = 300;
  static constexpr int ECHO_TIMEOUT = 3000;
  static constexpr double SOUND_SPEED = 0.034;
  static constexpr double WALL_THRESHOLD = 20.0;

  static constexpr int MAX_SPEED = 255;
  static constexpr int BASE_SPEED = 80;
  static constexpr int TURN_SPEED = 120;
  static constexpr int MOVE_DELAY = 500;
  static constexpr int TURN_DELAY = 500;

  Servo servo;

  void setMotors(int leftSpeed, int rightSpeed) {
    int absoluteLeftSpeed = clamp(abs(leftSpeed), 0, MAX_SPEED);
    int absoluteRightSpeed = clamp(abs(rightSpeed), 0, MAX_SPEED);

    digitalWrite(IN1, leftSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN2, leftSpeed < 0 ? HIGH : LOW);
    analogWrite(ENA, absoluteLeftSpeed);

    digitalWrite(IN3, rightSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN4, rightSpeed < 0 ? HIGH : LOW);
    analogWrite(ENB, absoluteRightSpeed);
  }

  void stopMotors() { setMotors(0, 0); }

  double measureDistance(int frontAngle = 0) {
    int angle = frontAngle + 90;
    servo.write(angle);
    delay(SERVO_DELAY);

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    double time = pulseIn(ECHO, HIGH, ECHO_TIMEOUT);
    double distance = time * SOUND_SPEED / 2;
    return distance;
  }

  WallState scanWalls() {
    WallState walls{
        measureDistance(-90) < WALL_THRESHOLD,
        measureDistance(0) < WALL_THRESHOLD,
        measureDistance(90) < WALL_THRESHOLD,
    };

    return walls;
  }

  void moveForward() {
    setMotors(BASE_SPEED, BASE_SPEED);
    delay(MOVE_DELAY);
  }

  void turnLeft() {
    setMotors(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DELAY);
  }

  void turnRight() {
    setMotors(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DELAY);
  }

  void adjustMotors(WallState walls) {
    if (!walls.left) {
      turnLeft();
    } else if (!walls.front) {
      moveForward();
    } else {
      turnRight();
    }
  }

public:
  void setup() override {
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    servo.attach(SERVO);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
  }

  void loop() override {
    auto walls = scanWalls();
    adjustMotors(walls);
  }
};
