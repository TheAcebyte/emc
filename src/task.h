#pragma once

class Task {
public:
  virtual ~Task() = default;
  virtual void setup() = 0;
  virtual void loop() = 0;
};
