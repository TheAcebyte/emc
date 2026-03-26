#include "task.h"
#include <Arduino.h>

template<typename T>
constexpr T clamp(T x, T min, T max) {
  if (x < min) return min;
  if (x > max) return max;
  return x;
}

struct SensorState {
  int left;
  int middle;
  int right;

  bool blank() const { return left == 0 && middle == 0 && right == 0; }
};

class LineFollower : public Task {
private:
  static constexpr int ENA = 6;
  static constexpr int IN1 = 9;
  static constexpr int IN2 = 10;

  static constexpr int ENB = 5;
  static constexpr int IN3 = 7;
  static constexpr int IN4 = 8;

  static constexpr int SENSOR_LEFT = A0;
  static constexpr int SENSOR_MIDDLE = A1;
  static constexpr int SENSOR_RIGHT = A2;

  static constexpr double MAX_INTEGRAL = 255;
  static constexpr double KE = 100;
  static constexpr double KP = 0.8;
  static constexpr double KI = 0.0;
  static constexpr double KD = 0.2;

  static constexpr int MAX_SPEED = 200;
  static constexpr int BASE_SPEED = 100;

  void setMotors(int leftSpeed, int rightSpeed) {
    int absoluteLeftSpeed = clamp(abs(leftSpeed), 0, MAX_SPEED);
    int absoluteRightSpeed = clamp(abs(rightSpeed), 0, MAX_SPEED);

    digitalWrite(IN1, leftSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN2, leftSpeed < 0 ? HIGH : LOW);
    analogWrite(ENA, absoluteLeftSpeed);

    digitalWrite(IN3, rightSpeed > 0 ? HIGH : LOW);
    digitalWrite(IN4, rightSpeed < 0 ? HIGH : LOW);
    analogWrite(ENB, absoluteRightSpeed);
  }

  void stopMotors() { setMotors(0, 0); }

  SensorState readSensors() {
    SensorState sensors{
        analogRead(SENSOR_LEFT),
        analogRead(SENSOR_MIDDLE),
        analogRead(SENSOR_RIGHT),
    };

    return sensors;
  }

  double calculateError(SensorState sensors) {
    double l = sensors.left;
    double m = sensors.middle;
    double r = sensors.right;

    return KE * (r - l) / (l + m + r);
  }

  int calculateTurn(double error) {
    static double integral = 0;
    static double lastError = 0;

    integral = clamp(integral + error, -MAX_INTEGRAL, MAX_INTEGRAL);
    double p = error;
    double i = integral;
    double d = error - lastError;
    lastError = error;

    int turn = KP * p + KI * i + KD * d;
    return turn;
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
    SensorState sensors = readSensors();
    int turn = 0;
    if (!sensors.blank()) {
      double error = calculateError(sensors);
      turn = calculateTurn(error);
    }

    int leftSpeed = BASE_SPEED + turn;
    int rightSpeed = BASE_SPEED - turn;
    setMotors(leftSpeed, rightSpeed);
  }
};
