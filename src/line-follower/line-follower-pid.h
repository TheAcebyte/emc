#pragma once

#include "config.h"
#include "task.h"
#include "types.h"
#include "utils.h"
#include <Arduino.h>

using namespace LineFollowerTypes;
using namespace LineFollowerPIDConfig;

class LineFollower : public Task {
private:
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

  SensorState readSensors() {
    SensorState sensors{
        digitalRead(SENSOR_LEFT) == HIGH,
        digitalRead(SENSOR_MIDDLE) == HIGH,
        digitalRead(SENSOR_RIGHT) == HIGH,
    };

    return sensors;
  }

  double calculateError(SensorState sensors) {
    double l = sensors.left;
    double m = sensors.middle;
    double r = sensors.right;

    double error = KE * (r - l) / (l + m + r);
    return error;
  }

  int calculateTurn(double error) {
    static double integral = 0;
    static double lastError = 0;

    integral = clamp(integral + error, -MAX_INTEGRAL, MAX_INTEGRAL);
    double p = error;
    double i = integral;
    double d = error - lastError;
    lastError = error;

    int turn = KP * p + KI * i + KD * d;
    return turn;
  }

  void adjustMotors(SensorState sensors) {
    int turn = 0;
    if (!sensors.blank()) {
      double error = calculateError(sensors);
      turn = calculateTurn(error);
    }

    int leftSpeed = BASE_SPEED - turn;
    int rightSpeed = BASE_SPEED + turn;
    setMotors(leftSpeed, rightSpeed);
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
