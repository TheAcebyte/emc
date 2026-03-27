#pragma once

#include <Arduino.h>
#include "task.h"

class DebugMotorTurn : public Task {
private:
  static constexpr int ENA = 6;
  static constexpr int IN1 = 9;
  static constexpr int IN2 = 10;

  static constexpr int ENB = 5;
  static constexpr int IN3 = 7;
  static constexpr int IN4 = 8;

  static constexpr int TURN_SPEED = 120;

public:
  void setup() override {
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
  }

  void loop() override {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, TURN_SPEED);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, TURN_SPEED);
  }
};
