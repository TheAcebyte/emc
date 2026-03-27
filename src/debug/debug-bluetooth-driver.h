#pragma once

#include <Arduino.h>
#include "task.h"

class DebugBluetoothDriver : public Task {
public:
  void setup() override {
    Serial.begin(9600);
    while (!Serial);
  }

  void loop() override {
    if (!Serial.available()) {
      return;
    }

    auto val = Serial.read();
    Serial.println(val);
  }
};
