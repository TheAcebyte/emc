#pragma once

#include "lifting-arm-controller.h"
#include "task.h"
#include "utils.h"
#include <Arduino.h>
#include <Servo.h>

class LiftingArm : Task {
private:
  static constexpr int ENA = 6;
  static constexpr int IN1 = 7;
  static constexpr int IN2 = 8;

  static constexpr int ENB = 5;
  static constexpr int IN3 = 4;
  static constexpr int IN4 = 12;

  static constexpr int MAX_SPEED = 100;

  static constexpr int ARM_SERVO = 10;
  static constexpr int CLAW_SERVO = 9;

  static constexpr int ARM_MIN_ANGLE = 0;
  static constexpr int ARM_MAX_ANGLE = 90;
  static constexpr int ARM_STEP_ANGLE = 5;

  static constexpr int CLAW_MIN_ANGLE = 0;
  static constexpr int CLAW_MAX_ANGLE = 90;
  static constexpr int CLAW_STEP_ANGLE = 5;

  static constexpr int TICK_DELAY = 20;

  LiftingArmController &controller = LiftingArmController::getController();
  Servo armServo;
  Servo clawServo;

  void setMotors(int leftSpeed, int rightSpeed) {
    digitalWrite(IN3, leftSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN4, leftSpeed < 0 ? HIGH : LOW);
    analogWrite(ENB, abs(leftSpeed));

    digitalWrite(IN1, rightSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN2, rightSpeed < 0 ? HIGH : LOW);
    analogWrite(ENA, abs(rightSpeed));
  }

  void stopMotors() { setMotors(0, 0); }

  void nudgeArmForward() {
    int newAngle = armServo.read() + ARM_STEP_ANGLE;
    int clampedAngle = clamp(newAngle, ARM_MIN_ANGLE, ARM_MAX_ANGLE);
    armServo.write(clampedAngle);
  }

  void nudgeArmBackward() {
    int newAngle = armServo.read() - ARM_STEP_ANGLE;
    int clampedAngle = clamp(newAngle, ARM_MIN_ANGLE, ARM_MAX_ANGLE);
    armServo.write(clampedAngle);
  }

  void nudgeClawIn() {
    int newAngle = clawServo.read() + CLAW_STEP_ANGLE;
    int clampedAngle = clamp(newAngle, CLAW_MIN_ANGLE, CLAW_MAX_ANGLE);
    clawServo.write(clampedAngle);
  }

  void nudgeClawOut() {
    int newAngle = clawServo.read() - CLAW_STEP_ANGLE;
    int clampedAngle = clamp(newAngle, CLAW_MIN_ANGLE, CLAW_MAX_ANGLE);
    clawServo.write(clampedAngle);
  }

  void adjustMotors() {
    double x = controller.getJoystickX();
    double y = controller.getJoystickY();
    double leftMultiplier = clamp(y + x, -1.0, 1.0);
    double rightMultiplier = clamp(y - x, -1.0, 1.0);

    int leftSpeed = leftMultiplier * MAX_SPEED;
    int rightSpeed = rightMultiplier * MAX_SPEED;
    setMotors(leftSpeed, rightSpeed);
  }

  void adjustServos() {
    if (controller.pushingArmForward()) {
      nudgeArmForward();
    } else if (controller.pushingArmBackward()) {
      nudgeArmBackward();
    }

    if (controller.pushingClawIn()) {
      nudgeClawIn();
    } else if (controller.pushingClawOut()) {
      nudgeClawOut();
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

    armServo.attach(ARM_SERVO);
    clawServo.attach(CLAW_SERVO);
  }

  void loop() override {
    controller.step();
    if (!controller.isConnected()) {
      stopMotors();
      return;
    }

    adjustMotors();
    adjustServos();
    delay(TICK_DELAY);
  }
};
