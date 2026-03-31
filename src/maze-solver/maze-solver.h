#pragma once

#include "config.h"
#include "task.h"
#include "utils.h"
#include <Arduino.h>
#include <PWMServo.h>

using namespace MazeSolverConfig;

struct WallState {
  double left;
  double front;
  double right;

  bool emptyLeft() const { return left > SIDE_WALL_THRESHOLD; }
  bool emptyFront() const { return front > FRONT_WALL_THRESHOLD; }
  bool emptyRight() const { return right > SIDE_WALL_THRESHOLD; }
  bool all() { return !emptyLeft() && !emptyFront() && !emptyRight(); }
};

class MazeSolver : Task {
private:
  PWMServo servo;
  int left = 0;
  int right = 0;

  void setMotors(int leftSpeed, int rightSpeed) {
    int absoluteLeftSpeed = clamp(abs(leftSpeed), 0, MAX_SPEED);
    int absoluteRightSpeed = clamp(abs(rightSpeed), 0, MAX_SPEED);

    digitalWrite(IN3, leftSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN4, leftSpeed < 0 ? HIGH : LOW);
    analogWrite(ENB, absoluteLeftSpeed);

    digitalWrite(IN1, rightSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN2, rightSpeed < 0 ? HIGH : LOW);
    analogWrite(ENA, absoluteRightSpeed);
  }

  void stopMotors() { setMotors(0, 0); }

  void resetServo() {
    servo.write(90);
    delay(SERVO_DELAY);
  }

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
    if (distance == 0) {
      return MAX_DISTANCE;
    }

    return distance;
  }

  WallState scanWalls() {
    WallState walls{
        measureDistance(90),
        measureDistance(0),
        measureDistance(-90),
    };

    resetServo();
    return walls;
  }

  double calculateError(WallState walls) {
    double l = walls.left;
    double r = walls.right;
    if (l + r == 0) {
      return 0;
    }

    double error = ERROR_SCALE * (r - l) / (l + r);
    return error;
  }

  void moveForward(WallState walls) {
    double error = 0;
    if (!walls.emptyLeft() && !walls.emptyRight()) {
      error = calculateError(walls);
    }

    int leftSpeed = BASE_SPEED + error;
    int rightSpeed = BASE_SPEED - error;
    setMotors(leftSpeed, rightSpeed);
    delay(MOVE_DELAY);
    stopMotors();
  }

  void turnLeft() {
    setMotors(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DELAY);
    stopMotors();
  }

  void turnRight() {
    setMotors(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DELAY);
    stopMotors();
  }

  void turnAround() {
    setMotors(TURN_SPEED, -TURN_SPEED);
    delay(2 * TURN_DELAY);
    stopMotors();
  }

  void adjustMotors(WallState walls) {
    if (walls.all()) {
      left = right = 0;
      turnAround();
      return;
    }

    if (walls.emptyLeft()) {
      ++left;
    } else {
      left = 0;
    }

    if (walls.emptyRight()) {
      ++right;
    } else {
      right = 0;
    }

    if (left >= TURN_THRESHOLD) {
      turnLeft();
      left = 0;
    } else if (right >= TURN_THRESHOLD && !walls.emptyFront()) {
      turnRight();
      right = 0;
    }

    moveForward(walls);
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
