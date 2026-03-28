#pragma once

#include "config.h"
#include <Arduino.h>

using namespace LiftingArmConfig;

#define REMOTEXY_MODE__SOFTSERIAL

#include <SoftwareSerial.h>

#define REMOTEXY_SERIAL_RX RX
#define REMOTEXY_SERIAL_TX TX
#define REMOTEXY_SERIAL_SPEED 9600

#include <RemoteXY.h>

#pragma pack(push, 1)
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] = // 45 bytes V19
    {255, 4,  0,  0,  0, 38, 0,  19,  0,  0,  0,  0,  31, 1,  106,
     200, 1,  1,  3,  0, 5,  22, 125, 60, 60, 32, 2,  26, 31, 3,
     19,  32, 16, 44, 3, 2,  26, 3,   68, 32, 16, 44, 3,  2,  26};

struct {
  int8_t joystickX;     // from -100 to 100
  int8_t joystickY;     // from -100 to 100
  uint8_t armSwitch;    // from 0 to 3
  uint8_t clawSwitch;   // from 0 to 3
  uint8_t connect_flag; // = 1 if wire connected, else = 0
} RemoteXY;
#pragma pack(pop)

constexpr int MAX_JOYSTICK = 100;

class LiftingArmController {
private:
  LiftingArmController() { RemoteXY_Init(); }

public:
  static LiftingArmController &getController() {
    static LiftingArmController controller;
    return controller;
  }

  LiftingArmController(const LiftingArmController &other) = delete;
  LiftingArmController operator=(const LiftingArmController &other) = delete;

  void step() { RemoteXYEngine.handler(); }
  bool isConnected() { return RemoteXY.connect_flag; }

  bool pushingArmForward() { return RemoteXY.armSwitch == 1; }
  bool pushingArmBackward() { return RemoteXY.armSwitch == 2; }

  bool pushingClawIn() { return RemoteXY.clawSwitch == 1; }
  bool pushingClawOut() { return RemoteXY.clawSwitch == 2; }

  double getJoystickX() { return (double)RemoteXY.joystickX / MAX_JOYSTICK; }
  double getJoystickY() { return (double)RemoteXY.joystickY / MAX_JOYSTICK; }
};
