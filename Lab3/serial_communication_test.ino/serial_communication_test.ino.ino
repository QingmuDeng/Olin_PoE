
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"


Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

int LED = 13;

void setup() {
  // put your setup code here, to run once:

  
  AFMS.begin();
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

  //Have arduino wait for input
  while(Serial.available() == 0);

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
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    
  }
  //Press 0 for LED off, MOTORS OFF
  if (val == 0)
  {
    rightMotor->run(RELEASE);
    leftMotor->run(RELEASE);
    Serial.println("STOP");
    digitalWrite(LED_BUILTIN, LOW);
  }
}
