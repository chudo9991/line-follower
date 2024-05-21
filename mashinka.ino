/*
	Пример управления двумя моторами при помощи одного джойстика по танковой схеме
*/
#include "GyverMotor.h"
GMotor motorR(DRIVER2WIRE, 2, 3, HIGH);
GMotor motorL(DRIVER2WIRE, 4, 11, HIGH);
// используем оба ШИМа таймера 2 (пины 3 и 11)
int a0, a1, a2, a3;
int speed = 200;
int maxspeed = 255;
int trsh = 160;
int step =7;

// варианты инициализации в зависимости от типа драйвера:
// GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW / HIGH) )
// GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) )
// GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) )
/*
  DRIVER2WIRE - двухпроводной драйвер (направление + ШИМ)
  DRIVER3WIRE - трёхпроводной драйвер (два пина направления + ШИМ)
  RELAY2WIRE - реле в качестве драйвера (два пина направления)

  dig_pin, dig_pin_A, dig_pin_B - любой цифровой пин МК
  PWM_pin - любой ШИМ пин МК
  LOW / HIGH - уровень драйвера. Если при увеличении скорости мотор наоборот тормозит - смени уровень
*/

void setup() {
  // ключ на старт!
  motorR.setMode(FORWARD);
  motorL.setMode(BACKWARD);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  Serial.begin(9600);
}

void loop() {

  motorR.setMode(FORWARD);
  motorL.setMode(BACKWARD);

  a0=analogRead(A0);
  a1=analogRead(A1);
  a2=analogRead(A2);
  a3=analogRead(A3);
  Serial.println(a0);
// a1 < 100 - белое, >100 черное
//
  if ((a0>trsh) && (a1<=trsh) or ((a0>=trsh) and (a2>=trsh))) { // a0 это справа
      motorR.setMode(BACKWARD);
      motorR.setSpeed(maxspeed); // этим мотором тормозим
      motorL.setSpeed(maxspeed);
      delay(step*5);
      motorR.setMode(FORWARD);
    } else if ((a1>trsh) && (a0<=trsh) or ((a1>=trsh) and (a3>=trsh))) { // a1 это слева
      motorL.setMode(FORWARD);
      motorR.setSpeed(maxspeed);
      motorL.setSpeed(maxspeed); // этим мотором тормозим
      delay(step*5); 
      motorL.setMode(BACKWARD);  
    } else if ((a2>trsh) && (a3<=trsh)) { //a2 это справа внутренний
      motorL.setMode(FORWARD);
      motorR.setSpeed(speed);
      motorL.setSpeed(speed); // этим мотором тормозим
      delay(step+2); 
      motorL.setMode(BACKWARD);  
    } else if ((a3>trsh) && (a2<=trsh)) { //a3 это слева внутренний
      motorR.setMode(BACKWARD);
      motorR.setSpeed(speed); // этим мотором тормозим
      motorL.setSpeed(speed);
      delay(step+2);
      motorR.setMode(FORWARD);
//    } else if ((a1>trsh) && (a0>trsh)) {
//      motorR.setSpeed(STOP);
//      motorL.setSpeed(STOP); 
//      delay(3);
    }  else { // if ((a1<trsh) && (a0<trsh)) {
      motorR.setSpeed(speed);
      motorL.setSpeed(speed);
      delay(step);  
    };
    if ((a1>=trsh)&&(a3>=trsh)&(a0>=trsh)&&(a2>=trsh)||(a1>=trsh)&&(a3>=trsh)&((a0>=trsh)||(a2>=trsh))||((a1>=trsh)||(a3>=trsh)&(a0>=trsh)&&(a2>=trsh))) {
      motorR.setSpeed(speed);
      motorL.setSpeed(speed);
      delay(step*4);  
    } ;
//  delay(200);  // задержка просто для "стабильности"
}
