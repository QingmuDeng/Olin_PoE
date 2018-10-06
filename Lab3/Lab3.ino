
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

void setup() {
  // put your setup code here, to run once:

  AFMS.begin();
  Serial.begin(9600);
  
}

void loop() {

  //Have arduino wait for input
  //while(Serial.available() == 0);
  

  //Read input
  int val = Serial.read() - '0';

  //Press 1 for LED ON, MOTORS FORWARD
  if (val == 1)
  {
    leftMotor->setSpeed(150);
    leftMotor ->run(FORWARD);
    rightMotor->setSpeed(150);
    rightMotor ->run(FORWARD);
    Serial.println("FORWARD");
    delay(500);
    
  }
  //Press 0 for LED off, MOTORS OFF
  if (val == 0)
  {
    rightMotor->run(RELEASE);
    leftMotor->run(RELEASE);
    Serial.println("STOP");
  }






// ANNA'S CODE   
//  // put your main code here, to run repeatedly:
//  leftMotor->setSpeed(150);
//  leftMotor ->run(FORWARD);
//  rightMotor->setSpeed(150);
//  rightMotor ->run(FORWARD);
//  delay(2000);
//
//  leftMotor->setSpeed(150);
//  leftMotor ->run(BACKWARD);
//  rightMotor->setSpeed(150);
//  rightMotor ->run(BACKWARD);
//  delay(2000);
//
////  rightMotor->run(RELEASE);
//  leftMotor->run(RELEASE);
//  delay(1000);
//
//  Serial.println("hi");
  
}
