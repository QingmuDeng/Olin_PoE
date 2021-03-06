// When can't connect to Arduino:
// sudo chmod a+rw /dev/ttyACM0
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "struct.h"


Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

/***** Global Variables *****/
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
/***** End of Global Variables *****/

/***** Serial Tuning State Machine *****/
volatile TuneState state;
/***** End of Serial Tuning State Machine *****/

/***** Sensing Struct *****/
Sensors sense;
void SensorUpdate(Sensors* mySense){
  // Read the two IR reflective sensors, calculate
  // their difference and the PID controller error
  mySense->sensor_left = analogRead(A0);
  mySense->sensor_right = analogRead(A1);
  mySense->diff = mySense->sensor_left - mySense->sensor_right;
  mySense->error = 0 - mySense->diff;
}
/***** End of Sensing Struct *****/

/***** Pid Controller Struct *****/
PidObject controller;
void PidInitialize(PidObject* pid){
  // Initiate the global controller with the following numbers
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
  // Pid update function that calcualtes PTerm and Iterm
  float pTerm, dTerm, iTerm;

  pTerm = pid->pGain * Mysense->error; // calculate the proportional terms

  pid->iState += Mysense->error;
  if(pid->iState > pid->iMax) pid->iState = pid->iMax;
  else if(pid->iState < pid->iMin) pid->iState = pid->iMin;

  iTerm = pid->iGain *  pid->iState;

  return pTerm + iTerm;
}
/***** End of Pid Controller Struct *****/


void MotorUpdate(int motor_diff){
  // Actuate the motors based on the terms generated by PID controller
  leftMotor->setSpeed(35-(motor_diff>>1));
  rightMotor->setSpeed(35+(motor_diff>>1));
  leftSpeed = 35-(motor_diff>>1);
  rightSpeed = 35+(motor_diff>>1);
  leftMotor ->run(FORWARD);
  rightMotor ->run(FORWARD);
}

void setup() {
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

  // Actuate Motor
  MotorUpdate(motor_diff);

  // Serial tuning state machine
  /*  Usage: 
  1. To start with, the serial tune is in the "MENU" state. Sned 'p', 'i', or 'd'
  to enter the tuning states for proportional, integral, and derivative gain tuning.
  2. Once you are in one of the tuning states, type in a number that is not 1000 to
  make change the gain of the current term to that number.
  3. Type in 1000 to move back to "MENU" state in order to tune a different gain  
  */
  if (Serial.available()){ // Check if serial input.
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
    // Time steps for recoding
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
