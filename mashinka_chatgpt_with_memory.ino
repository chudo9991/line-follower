#include "GyverMotor.h"

GMotor motorR(DRIVER2WIRE, 2, 3, HIGH);
GMotor motorL(DRIVER2WIRE, 4, 11, HIGH);

int a0, a1, a2, a3;
int speed = 100;
int maxspeed = 255;
int trsh = 160;
int step = 7;

// Переменные для хранения состояния манёвра
enum Maneuver { STRAIGHT, TURN_RIGHT, TURN_LEFT, ADJUST_RIGHT, ADJUST_LEFT, CROSSROAD };
Maneuver currentManeuver = STRAIGHT;

void setup() {
  motorR.setMode(FORWARD);
  motorL.setMode(BACKWARD);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  Serial.begin(9600);
}

void loop() {
  a0 = analogRead(A0);
  a1 = analogRead(A1);
  a2 = analogRead(A2);
  a3 = analogRead(A3);

  // Проверка перекрестка
  if (a0 >= trsh && a1 >= trsh && a2 >= trsh && a3 >= trsh) {
    handleCrossroad();
    return;
  }

  // Логика манёвра
  if (a0 > trsh && a1 <= trsh && a2 <= trsh) { // Правый край
    turnRight();
  } else if (a1 > trsh && a0 <= trsh && a3 <= trsh) { // Левый край
    turnLeft();
  } else if (a2 > trsh && a3 <= trsh) { // Правый внутренний
    adjustRight();
  } else if (a3 > trsh && a2 <= trsh) { // Левый внутренний
    adjustLeft();
  } else if (a2 > trsh && a3 > trsh && (a0 > trsh || a1 > trsh)) {
    handleCurrentManeuver();
  } else { // Все датчики на белом
    goStraight();
  }
}

void goStraight() {
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  delay(step);
  currentManeuver = STRAIGHT;
}

void turnRight() {
  motorR.setMode(BACKWARD);
  motorR.setSpeed(maxspeed); 
  motorL.setSpeed(maxspeed);
  delay(step * 5);
  motorR.setMode(FORWARD);
  currentManeuver = TURN_RIGHT;
}

void turnLeft() {
  motorL.setMode(FORWARD);
  motorR.setSpeed(maxspeed);
  motorL.setSpeed(maxspeed);
  delay(step * 5); 
  motorL.setMode(BACKWARD);
  currentManeuver = TURN_LEFT;
}

void adjustRight() {
  motorL.setMode(FORWARD);
  motorR.setSpeed(speed);
  motorL.setSpeed(speed); 
  delay(step + 2); 
  motorL.setMode(BACKWARD);
  currentManeuver = ADJUST_RIGHT;
}

void adjustLeft() {
  motorR.setMode(BACKWARD);
  motorR.setSpeed(speed); 
  motorL.setSpeed(speed);
  delay(step + 2);
  motorR.setMode(FORWARD);
  currentManeuver = ADJUST_LEFT;
}

void handleCrossroad() {
  motorR.setSpeed(0);
  motorL.setSpeed(0);
  delay(step * 10); // Задержка для стабилизации
  motorR.setSpeed(speed);
  motorL.setSpeed(speed);
  delay(step * 10); // Время для проезда перекрестка
  currentManeuver = CROSSROAD;
}

void handleCurrentManeuver() {
  if (currentManeuver == TURN_RIGHT) {
    turnRight();
  } else if (currentManeuver == TURN_LEFT) {
    turnLeft();
  } else if (currentManeuver == ADJUST_RIGHT) {
    adjustRight();
  } else if (currentManeuver == ADJUST_LEFT) {
    adjustLeft();
  }
}
