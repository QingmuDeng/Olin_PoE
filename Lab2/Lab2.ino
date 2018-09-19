#include <Servo.h>

Servo pan;
uint8_t pan_angle=0;
Servo tilt;
uint8_t tilt_angle=60;
const int sweep_increment = 4;
const int sweep_delay = 75;

const int infrared = A0;
const int measure_delay = 110; // 38.3+9.6+5.0 = 52.9
int sensorVal = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115520);
  pan.attach(10);
  tilt.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  pan_tilt_sweep();//
  take_measure();
  print_to_laptop();
  Serial.println(sensorVal);
}

void pan_sweep(){
  pan.write(pan_angle);
  if(pan_angle > 180){
    pan_angle = 0;
  }
}

void tilt_sweep(){
  tilt.write(tilt_angle);
  tilt_angle += sweep_increment;
  if(tilt_angle > 120){
    tilt_angle = 60;
    pan_angle += 2;//sweep_increment;
  }
}

void pan_tilt_sweep(){
  tilt_sweep();
  pan_sweep();
  delay(sweep_delay);
}

void take_measure(){
  delay(measure_delay);
  sensorVal = analogRead(A0);
}

void print_to_laptop(){
  Serial.print("pan = ");  Serial.print(pan_angle);  Serial.print("    ");
  Serial.print("tilt = ");  Serial.print(tilt_angle);  Serial.print("    ");
  Serial.print("output = ");  Serial.println(sensorVal);
}

void serialEvent(){
  //statements
  
}


