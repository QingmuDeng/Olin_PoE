// When can't connect to Arduino:
// sudo chmod a+rw /dev/ttyACM0
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(3);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(4);

int max_speed = 50; // max speed 255

typedef enum{
  Tune_p,
  Tune_i,
  Tune_d,
  Tune_m, //Menu to choose p, i, or d. It's the "no state selected state"
} TuneState;
volatile TuneState state;

typedef struct {
  int sensor_left, sensor_right, diff, error;
} Sensors;
Sensors sense;
void SensorUpdate(&Sensors mySense){
  mySense->sensor_left = analogRead(A0);
  mySense->sensor_right = analogRead(A1);
  mySense->diff = mySense->sensor_left - mySense->sensor_right;
  mySense->error = 0 - mySense->diff;
}

typedef struct {
  double iGain,
         pGain,
         dGain;

  double dState,
         iState,
         iMin,
         iMax;
} PidObject;
PidObject controller;

void PidInitialize(PidObject* pid){
  PidObject pid_init;
  pid_init.pGain = 0.05;
  pid_init.iGain = 0.0001;
  pid_init.dGain = 0;
  pid_init.iMin = -700;
  pid_init.iMax = 700;

  pid->iGain = pid_init.iGain;
  pid->pGain = pid_init.pGain;
  pid->dGain = pid_init.dGain;
  pid->iMin = pid_init.iMin;
  pid->iMax = pid_init.iMax;
}

double PidUpdate(PidObject* pid, double error, double position)
{
  double pTerm, dTerm, iTerm;

  pTerm = pid->pGain * error; // calculate the proportional terms

  pid->iState += error;
  if(pid->iState > pid->iMax) pid->iState = pid->iMax;
  else if(pid->iState < pid->iMin) pid->iState = pid->iMin;

  iTerm = pid->iGain *  pid->iState;

  return pTerm + iTerm;
}

void MotorUpdate(int motor_diff){
  leftMotor->setSpeed(150);
  rightMotor->setSpeed(150+motor_diff);
  leftMotor ->run(FORWARD);
  rightMotor ->run(FORWARD);
  Serial.println("FORWARD");
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
  double motor_diff = PidUpdate(&controller, &sense);
  Serial.println(motor_diff);
  /** Motor Code **/
  MotorUpdate(motor_diff);
  /** End of Motor Code **/

  // Check if serial input.
  if (Serial.available()){
    switch (state){
      case Tune_p:
        int change = Serial.parseInt();         //if state == Tune_p, read serial with Serial.parseInt() [takes number with more than one digit] and change corresponding gain
        if (change == 1000){
          state = Tune_m;
        }else{
          Serial.print("P changed by "); Serial.println(change);
        }
      case Tune_i:
        int change = Serial.parseInt();
        if (change == 1000){
          state = Tune_m;
        }else{
          Serial.print("I changed by "); Serial.println(change);
        }
      case Tune_d:
        int change = Serial.parseInt();
        if (change == 1000){
          state = Tune_m;
        }else{
          Serial.print("D changed by "); Serial.println(change);
        }
      case Tune_m:                              //If state == Tune_m, check serial input for 'p','i', or 'd' and change state to it
        int val = Serial.read();
        if (val == (int)'p'){
          state = Tune_p;
          Serial.print("Changed to state P")
        }else if (val == (int)'i'){
          state = Tune_i;
          Serial.print("Changed to state I")
        }else if (val == (int)'d'){
          state = Tune_d;
          Serial.print("Changed to state D")
        }
    }
  }
}
