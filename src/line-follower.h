#include "task.h"
#include <Arduino.h>

enum class Movement {
  FORWARD,
  LEFT,
  RIGHT,
  STOP,
};

struct MotorSpeed {
  int left;
  int right;
};

struct MovementMapping {
  Movement movement;
  MotorSpeed speed;
};

struct SensorState {
  bool left;
  bool middle;
  bool right;

  bool operator==(const SensorState &other) const {
    return left == other.left && middle == other.middle && right == other.right;
  }
};

struct SensorMapping {
  SensorState sensors;
  Movement movement;
};

class LineFollower : public Task {
private:
  static constexpr int ENA = 6;
  static constexpr int IN1 = 9;
  static constexpr int IN2 = 10;

  static constexpr int ENB = 5;
  static constexpr int IN3 = 7;
  static constexpr int IN4 = 8;

  static constexpr int SENSOR_LEFT = 13;
  static constexpr int SENSOR_MIDDLE = 2;
  static constexpr int SENSOR_RIGHT = 12;

  static constexpr int BASE_SPEED = 70;
  static constexpr int TURN_SPEED = 140;

  static inline constexpr MovementMapping MOVEMENT_MAPPINGS[] = {
    {Movement::FORWARD, {.left =  BASE_SPEED, .right =  BASE_SPEED}},
    {Movement::LEFT,    {.left = -TURN_SPEED, .right =  TURN_SPEED}},
    {Movement::RIGHT,   {.left =  TURN_SPEED, .right = -TURN_SPEED}},
    {Movement::STOP,    {.left =  0,          .right =  0}},
  };

  static inline constexpr SensorMapping SENSOR_MAPPINGS[] = {
    {{.left = false, .middle = true,  .right = false}, Movement::FORWARD},
    {{.left = true,  .middle = true,  .right = true},  Movement::FORWARD},
    {{.left = true,  .middle = true,  .right = false}, Movement::LEFT},
    {{.left = false, .middle = true,  .right = true},  Movement::RIGHT},
    {{.left = true,  .middle = false, .right = false}, Movement::LEFT},
    {{.left = false, .middle = false, .right = true},  Movement::RIGHT},
  };

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
    pinMode(SENSOR_LEFT, INPUT);
    pinMode(SENSOR_MIDDLE, INPUT);
    pinMode(SENSOR_RIGHT, INPUT);

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
  }

  void loop() override {
    auto sensors = readSensors();
    adjustMotors(sensors);
  }
};
