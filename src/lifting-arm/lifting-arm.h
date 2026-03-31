#pragma once

#include "config.h"
#include "lifting-arm-controller.h"
#include "task.h"
#include "utils.h"
#include <Arduino.h>
#include <PWMServo.h>

using namespace LiftingArmConfig;

struct ServoMotor {
  PWMServo servo;
  int angle;
};

class LiftingArm : Task {
private:
  LiftingArmController &controller = LiftingArmController::getController();
  ServoMotor arm;
  ServoMotor claw;

  void setMotors(int leftSpeed, int rightSpeed) {
    digitalWrite(IN3, leftSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN4, leftSpeed < 0 ? HIGH : LOW);
    analogWrite(ENB, abs(leftSpeed));

    digitalWrite(IN1, rightSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN2, rightSpeed < 0 ? HIGH : LOW);
    analogWrite(ENA, abs(rightSpeed));
  }

  void stopMotors() { setMotors(0, 0); }

  int clampArmAngle(int angle) {
    return clamp(angle, ARM_MIN_ANGLE, ARM_MAX_ANGLE);
  }

  void nudgeArmForward() {
    int angle = arm.angle;
    int newAngle = clampArmAngle(angle + ARM_STEP_ANGLE);
    if (newAngle != angle) {
      arm.servo.write(newAngle);
      arm.angle = newAngle;
    }
  }

  void nudgeArmBackward() {
    int angle = arm.angle;
    int newAngle = clampArmAngle(angle - ARM_STEP_ANGLE);
    if (newAngle != angle) {
      arm.servo.write(newAngle);
      arm.angle = newAngle;
    }
  }

  int clampClawAngle(int angle) {
    return clamp(angle, CLAW_MIN_ANGLE, CLAW_MAX_ANGLE);
  }

  void nudgeClawIn() {
    int angle = claw.angle;
    int newAngle = clampClawAngle(angle + CLAW_STEP_ANGLE);
    if (newAngle != angle) {
      claw.servo.write(newAngle);
      claw.angle = newAngle;
    }
  }

  void nudgeClawOut() {
    int angle = claw.angle;
    int newAngle = clampClawAngle(angle - CLAW_STEP_ANGLE);
    if (newAngle != angle) {
      claw.servo.write(newAngle);
      claw.angle = newAngle;
    }
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

    arm.servo.attach(ARM_SERVO);
    arm.servo.write(ARM_MIN_ANGLE);

    claw.servo.attach(CLAW_SERVO);
    claw.servo.write(CLAW_MIN_ANGLE);
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
