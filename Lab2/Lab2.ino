#include <Servo.h>

Servo pan;
uint8_t pan_angle=60;
Servo tilt;
uint8_t tilt_angle=60;
bool tilt_direction=true;
const int sweep_increment = 2;
const int sweep_delay = 75;
const int infrared = A0;
const int measure_delay = 265; // 5*(38.3+9.6+5.0) to reach a stable reading
int sensorVal = 0;

typedef enum{
  Initialize,
  Scanning,
} State;
volatile State state;

void setup() {
  // put your setup code here, to run once:
  state = Initialize;
  Serial.begin(115520);
  pan.attach(10);
  tilt.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(state == Initialize){
    pan_angle = 60;
    tilt_angle=60;
    pan.write(pan_angle);
    tilt.write(tilt_angle);
  }else if(state == Scanning){
    pan_tilt_sweep();
    take_measure();
    print_to_laptop();
  }
}

void pan_sweep(){
  /* Pan sweep definition */
  if(pan_angle > 90){
    pan_angle = 60;
  }
  pan.write(pan_angle);
}

void tilt_sweep(){
  /* Tilt sweep definition */
  if(tilt_direction){
    tilt_angle += sweep_increment;
  }else{
    tilt_angle -= sweep_increment;
  }
  if(tilt_angle > 120){
    tilt_direction = false;
    pan_angle += sweep_increment;
  }else if(tilt_angle<60){
    tilt_direction = true;
    pan_angle += sweep_increment;
  }
  tilt.write(tilt_angle);
}

void pan_tilt_sweep(){ 
  /* Sweep with delay to allow servo to move into position */
  tilt_sweep();
  pan_sweep();
  delay(sweep_delay);
}

void take_measure(){
  /* Delay 5 times the measurment period before taking ADC reading */
  delay(measure_delay);
  sensorVal = analogRead(A0);
}

void print_to_laptop(){
  Serial.print("pan = ");  Serial.print(pan_angle);  Serial.print("    ");
  Serial.print("tilt = ");  Serial.print(tilt_angle);  Serial.print("    ");
  Serial.print("output = ");  Serial.print(sensorVal); Serial.print("    "); Serial.println(state);
}

void serialEvent(){
  //statements
  while (Serial.available()) {
    int a = Serial.read();
    if(a == (int)'i'){
      state = Scanning;
      break;
    }else if(a == (int)'e'){
      state = Initialize;
      break;
    }
  }
}


