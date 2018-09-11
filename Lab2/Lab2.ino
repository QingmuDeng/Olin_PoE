
#include <Servo.h>

Servo servo1;

const int analogInPin = A0;
const int analogOutPin = 9;

int sensorVal = 0;
int outputVal = 0;

int potpin = 0;
int val;




void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  servo1.attach(10);

}

void loop() {
  // put your main code here, to run repeatedly:

  sensorVal = analogRead(potpin);
  outputVal = map(sensorVal, 0, 1023, 0, 180);
  servo1.write(outputVal);
  delay(15);

  // analogWrite(analogOutPin, outputValue);

  Serial.print("sensor = ");  Serial.print(sensorVal);  Serial.print(",");
  Serial.print("output = ");  Serial.println(outputVal);

  delay(400);

  

}
