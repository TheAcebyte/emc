#pragma once

#include "task.h"
#include <Arduino.h>

class DebugMotorTurn : public Task {
private:
  static constexpr int ENA = 6;
  static constexpr int IN1 = 2;
  static constexpr int IN2 = 4;
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
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, TURN_SPEED);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, TURN_SPEED);
  }
};
