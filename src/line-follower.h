#pragma once

#include "task.h"
#include <Arduino.h>

class LineFollower : public Task {
public:
  void setup() override { 
    Serial.begin(9600);
    while (!Serial);
  }

  void loop() override {
    Serial.println("Hello, World!");
    delay(2000);
  }
};
