// ChatGPT4-o version

// Основные изменения:
//
// Добавлена проверка на нахождение всех датчиков на черном (перекресток).
// В случае перекрестка робот останавливается на некоторое время для стабилизации, затем продолжает движение.
// Логика следования по линии улучшена для более четкой работы моторов.

#include "GyverMotor.h"

GMotor motorR(DRIVER2WIRE, 2, 3, HIGH);
GMotor motorL(DRIVER2WIRE, 4, 11, HIGH);

int a0, a1, a2, a3;
int speed = 200;
int maxspeed = 255;
int trsh = 160;
int step = 7;

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
    // На перекрестке: остановка и выравнивание
    motorR.setSpeed(0);
    motorL.setSpeed(0);
    delay(step * 10); // Задержка для стабилизации
    motorR.setSpeed(speed);
    motorL.setSpeed(speed);
    delay(step * 10); // Время для проезда перекрестка
    return;
  }

  // Условия для следования по линии
  if (a0 > trsh && a1 <= trsh && a2 <= trsh) { // Правый край
    motorR.setMode(BACKWARD);
    motorR.setSpeed(maxspeed); 
    motorL.setSpeed(maxspeed);
    delay(step * 5);
    motorR.setMode(FORWARD);
  } else if (a1 > trsh && a0 <= trsh && a3 <= trsh) { // Левый край
    motorL.setMode(FORWARD);
    motorR.setSpeed(maxspeed);
    motorL.setSpeed(maxspeed);
    delay(step * 5); 
    motorL.setMode(BACKWARD);  
  } else if (a2 > trsh && a3 <= trsh) { // Правый внутренний
    motorL.setMode(FORWARD);
    motorR.setSpeed(speed);
    motorL.setSpeed(speed);
    delay(step + 2); 
    motorL.setMode(BACKWARD);  
  } else if (a3 > trsh && a2 <= trsh) { // Левый внутренний
    motorR.setMode(BACKWARD);
    motorR.setSpeed(speed); 
    motorL.setSpeed(speed);
    delay(step + 2);
    motorR.setMode(FORWARD);
  } else { // Все датчики на белом
    motorR.setSpeed(speed);
    motorL.setSpeed(speed);
    delay(step);  
  }
}
