#pragma once

#include "task.h"
#include <Arduino.h>

enum class Movement {
  FORWARD,
  LEFT,
  RIGHT,
  STOP,
};

struct SensorState {
  bool left;
  bool right;
};

class LineFollower : public Task {
private:
  static constexpr int ENA = 6;
  static constexpr int IN1 = 9;
  static constexpr int IN2 = 10;

  static constexpr int ENB = 5;
  static constexpr int IN3 = 7;
  static constexpr int IN4 = 8;

  static constexpr int SENSOR_LEFT = 13;
  static constexpr int SENSOR_RIGHT = 12;

  static constexpr int BASE_SPEED = 70;
  static constexpr int TURN_SPEED = 35;

  void setMotors(int leftSpeed, int rightSpeed) {
    digitalWrite(IN1, leftSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN2, leftSpeed < 0 ? HIGH : LOW);
    analogWrite(ENA, abs(leftSpeed));

    digitalWrite(IN3, rightSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN4, rightSpeed < 0 ? HIGH : LOW);
    analogWrite(ENB, abs(rightSpeed));
  }

  void stopMotors() { setMotors(0, 0); }

  void setMotors(Movement movement) {
    switch (movement) {
    case Movement::FORWARD:
      setMotors(BASE_SPEED, BASE_SPEED);
      break;

    case Movement::LEFT:
      setMotors(-BASE_SPEED, BASE_SPEED);
      break;

    case Movement::RIGHT:
      setMotors(BASE_SPEED, -BASE_SPEED);
      break;

    case Movement::STOP:
      stopMotors();
      break;
    }
  }

  SensorState readSensors() {
    SensorState sensors = {
        digitalRead(SENSOR_LEFT) == HIGH,
        digitalRead(SENSOR_RIGHT) == HIGH,
    };

    return sensors;
  }

  void adjustMotors() {
    auto sensors = readSensors();
    if (sensors.left && !sensors.right) {
      setMotors(Movement::LEFT);
    } else if (!sensors.left && sensors.right) {
      setMotors(Movement::RIGHT);
    } else {
      setMotors(Movement::FORWARD);
    }
  }

public:
  void setup() override {
    pinMode(SENSOR_LEFT, INPUT);
    pinMode(SENSOR_RIGHT, INPUT);

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
  }

  void loop() override {
    adjustMotors();
  }
};
