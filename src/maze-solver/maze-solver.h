#pragma once

#include "config.h"
#include "task.h"
#include "utils.h"
#include <Arduino.h>
#include <Servo.h>

using namespace MazeSolverConfig;

struct WallState {
  bool left;
  bool front;
  bool right;
};

class MazeSolver : Task {
private:
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
