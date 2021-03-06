const int GREEN = 6;
const int RED = 9;
const int BLUE = 10;
const int YELLOW = 11;
const int INFRA = A0;
const int BUTTON = 2;

int val = 0;
int old_val = 0;
int dist = 0;

// Tally the number of button push
int temp = 0;

// Declear states given out int differnt functions
void all_on();
void all_off();
void flashing();
void one_at_a_time();
void dj_set();


void setup() {
  // Set LED pins to be output and the button pin to the input
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);

  pinMode(BUTTON, INPUT);

  // Enable Serial Port with a 9600 baud rate
  Serial.begin(9600);

  // Set up External Interrupt on Pin 2 with a rising edge
  attachInterrupt(digitalPinToInterrupt(BUTTON), button_push, RISING);
}

void loop() {
  // store analog read value to dist
  dist = analogRead(INFRA);

  // Cycle through different mode using modulus
  if (temp % 5 == 1)
  {
    all_on();
  }
  else if (temp % 5 == 2)
  {
    flashing();
  }
  else if (temp % 5 == 3)
  {
    one_at_a_time();
  }
  else if (temp % 5 == 4)
  {
    dj_set();
  }
  else if (temp % 5 == 0)
  {
    all_off();
  }

  Serial.println(dist);
}

void button_push() {
  // Tally button press interrupt at every rising edge with 100 ms debounce
  temp++;
  delay(100);
}

void all_on() {
  // All on mode where the brightness of the leds are dependent upon the analog read value
  analogWrite(GREEN, dist/4);
  analogWrite(RED, dist/4);
  analogWrite(BLUE, dist/4);
  analogWrite(YELLOW, dist/4);
}

void all_off() {  
  // Turn off all leds
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);
  
}

void flashing() {
  // Flashing leds with dist milliseconds delay
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(YELLOW, HIGH);

  delay(dist);

  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);

  delay(dist);
}

void one_at_a_time() {
  // Cycle through one led at a time and delay dist milliseconds
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);

  delay(dist);
  dist = analogRead(INFRA);

  digitalWrite(GREEN, LOW);
  digitalWrite(RED, HIGH);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);

  delay(dist);
  dist = analogRead(INFRA);

  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, HIGH);
  digitalWrite(YELLOW, LOW);

  delay(dist);
  dist = analogRead(INFRA);

  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, HIGH);

  delay(dist);
  
}

void dj_set() {
  // Randomly light up leds and delay dist milliseconds
  int random_1 = 0;
  int random_2 = 0;
  int random_3 = 0;
  int random_4 = 0;

  random_1 = random(0, 2);
  random_2 = random(0, 2);
  random_3 = random(0, 2);
  random_4 = random(0, 2);

  digitalWrite(GREEN, random_1);
  digitalWrite(RED, random_2);
  digitalWrite(BLUE, random_3);
  digitalWrite(YELLOW, random_4);

  delay(dist);
}
