
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);

void setup() {
  // put your setup code here, to run once:

  AFMS.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  leftMotor->setSpeed(150);
  leftMotor ->run(FORWARD);
  rightMotor->setSpeed(150);
  rightMotor ->run(FORWARD);
  delay(2000);

  leftMotor->setSpeed(150);
  leftMotor ->run(BACKWARD);
  rightMotor->setSpeed(150);
  rightMotor ->run(BACKWARD);
  delay(2000);

  rightMotor->run(RELEASE);
  leftMotor->run(RELEASE);
  delay(1000);
  
}
