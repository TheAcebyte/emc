#pragma once

namespace LineFollowerTypes {
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

  bool blank() const { return !left && !middle && !right; }
};

struct SensorMapping {
  SensorState sensors;
  Movement movement;
};
}; // namespace LineFollowerTypes
