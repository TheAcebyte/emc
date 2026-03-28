#pragma once

#include "config.h"
#include "task.h"
#include "types.h"
#include <Arduino.h>

using namespace LineFollowerTypes;
using namespace LineFollowerConfig;

class LineFollower : public Task {
private:
  void setMotors(int leftSpeed, int rightSpeed) {
    digitalWrite(IN1, leftSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN2, leftSpeed < 0 ? HIGH : LOW);
    analogWrite(ENA, abs(leftSpeed));

    digitalWrite(IN3, rightSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN4, rightSpeed < 0 ? HIGH : LOW);
    analogWrite(ENB, abs(rightSpeed));
  }

  void stopMotors() { setMotors(0, 0); }

  void setMotors(Movement targetMovement) {
    for (auto [movement, speed] : MOVEMENT_MAPPINGS) {
      if (movement == targetMovement) {
        setMotors(speed.left, speed.right);
        return;
      }
    }
  }

  SensorState readSensors() {
    SensorState sensors = {
        digitalRead(SENSOR_LEFT) == HIGH,
        digitalRead(SENSOR_MIDDLE) == HIGH,
        digitalRead(SENSOR_RIGHT) == HIGH,
    };

    return sensors;
  }

  void adjustMotors(SensorState state) {
    static Movement lastMovement = Movement::STOP;

    for (auto [sensors, movement] : SENSOR_MAPPINGS) {
      if (state == sensors) {
        setMotors(movement);
        lastMovement = movement;
        return;
      }
    }

    setMotors(lastMovement);
  }

public:
  void setup() override {
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(SENSOR_LEFT, INPUT);
    pinMode(SENSOR_MIDDLE, INPUT);
    pinMode(SENSOR_RIGHT, INPUT);
  }

  void loop() override {
    auto sensors = readSensors();
    adjustMotors(sensors);
  }
};
