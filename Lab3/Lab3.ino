// When can't connect to Arduino:
// sudo chmod a+rw /dev/ttyACM0
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "struct.h"
#include <EEPROM.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

float change;
String input;
int val;
int max_speed = 50; // max speed 255
unsigned int addr = 0x01;
int rightSpeed;
int leftSpeed;
unsigned long startTime = millis();
unsigned long currentTime = millis();
unsigned long elapsedTime;


volatile TuneState state;

Sensors sense;
void SensorUpdate(Sensors* mySense){
  mySense->sensor_left = analogRead(A0);
  mySense->sensor_right = analogRead(A1);
  mySense->diff = mySense->sensor_left - mySense->sensor_right;
  mySense->error = 0 - mySense->diff;
}

PidObject controller;
void PidInitialize(PidObject* pid){
  PidObject pid_init;
  pid_init.pGain = 0.022;
  pid_init.iGain = 0.095;
  pid_init.dGain = 0;
  pid_init.iMin = -500;
  pid_init.iMax = 500;



  pid->iGain = pid_init.iGain;
  pid->pGain = pid_init.pGain;
  pid->dGain = pid_init.dGain;
  pid->iMin = pid_init.iMin;
  pid->iMax = pid_init.iMax;
}

float PidUpdate(PidObject* pid, Sensors* Mysense)
{
  float pTerm, dTerm, iTerm;

  pTerm = pid->pGain * Mysense->error; // calculate the proportional terms

  pid->iState += Mysense->error;
  if(pid->iState > pid->iMax) pid->iState = pid->iMax;
  else if(pid->iState < pid->iMin) pid->iState = pid->iMin;

  iTerm = pid->iGain *  pid->iState;

  return pTerm + iTerm;
}

void MotorUpdate(int motor_diff){
  leftMotor->setSpeed(35-(motor_diff>>1));
  rightMotor->setSpeed(35+(motor_diff>>1));
  leftSpeed = 35-(motor_diff>>1);
  rightSpeed = 35+(motor_diff>>1);
  leftMotor ->run(FORWARD);
  rightMotor ->run(FORWARD);
//  Serial.println("FORWARD");
}

void setup() {
  // put your setup code here, to run once:;
  AFMS.begin();
  Serial.begin(115520);
  PidInitialize(&controller);
  state = Tune_m;

}


void loop() {
  // Update sense struct accordingly
  SensorUpdate(&sense);

  // Calculate the difference between two motors to turn
  float motor_diff = PidUpdate(&controller, &sense);
//  Serial.println(motor_diff);
  /** Motor Code **/
  MotorUpdate(motor_diff);
  /** End of Motor Code **/

  // Check if serial input.
  if (Serial.available()){
    switch (state){
      case Tune_p:
        input = Serial.readString();
        change = input.toFloat();
        Serial.println(input.toFloat());      //if state == Tune_p, read serial with Serial.parseInt() [takes number with more than one digit] and change corresponding gain
        if (change == 1000){
          state = Tune_m;
          Serial.println("BACK IN MENU");
        }else if (change > 0){
          controller.pGain = change;
          Serial.print("P changed to "); Serial.println(input);
        }
        break;
      case Tune_i:
      input = Serial.readString();
      change = input.toFloat();
        if (change == 1000){
          state = Tune_m;
          Serial.println("BACK INMENU");
        }else if (change > 0){
          controller.iGain = change;
          Serial.print("I changed by "); Serial.println(change);
        }
        break;
      case Tune_d:
        input = Serial.readString();
        change = input.toFloat();
        if (change == 1000){
          state = Tune_m;
          Serial.println("BACK IN MENU");
        }else if (change > 0){
          controller.dGain = change;
          Serial.print("D changed by "); Serial.println(change);
        }
        break;
      case Tune_m:                              //If state == Tune_m, check serial input for 'p','i', or 'd' and change state to it
        val = Serial.read();
        if (val == (int)'p'){
          state = Tune_p;
          Serial.println("Changed to state P");
        }else if (val == (int)'i'){
          state = Tune_i;
          Serial.println("Changed to state I");
        }else if (val == (int)'d'){
          state = Tune_d;
          Serial.println("Changed to state D");
        }
        break;
    }

  }
  currentTime = millis();
  elapsedTime = currentTime - startTime;
  if (elapsedTime > 50) {
    print_to_laptop();
    startTime= currentTime;
  }


}



void print_to_laptop(){
  Serial.print("left motor = ");  Serial.print(leftSpeed);  Serial.print("    ");
  Serial.print("right motor = ");  Serial.print(rightSpeed);  Serial.print("    ");
  Serial.print("left sensor = ");  Serial.print(sense.sensor_left); Serial.print("    ");
  Serial.print("right sensor= "); Serial.print(sense.sensor_right);  Serial.print("    ");
  Serial.print("time= "); Serial.println(currentTime);
}
