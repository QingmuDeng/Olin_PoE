const int GREEN = 6;
const int RED = 9;
const int BLUE = 10;
const int YELLOW = 11;
const int INFRA = A0;

int val = 0;
int old_val = 0;
int dist = 0;

const int BUTTON = 8;

int temp = 0;


void all_on();
void all_off();
void flashing();
void one_at_a_time();
void dj_set();


void setup() {
  // put your setup code here, to run once:
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);

  pinMode(BUTTON, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  val = digitalRead(BUTTON);

  dist = analogRead(INFRA);
  


  

  if (old_val == 0 && val == 1)
  {
     temp++;
  }

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

  old_val = val;
  
}

void all_on() {

  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(YELLOW, HIGH);
}

void all_off() {

  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);
  
}

void flashing() {
  
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(YELLOW, HIGH);

  delay(100);

  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);

  delay(100);
}

void one_at_a_time() {

  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);

  delay(100);

  digitalWrite(GREEN, LOW);
  digitalWrite(RED, HIGH);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);

  delay(100);

  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, HIGH);
  digitalWrite(YELLOW, LOW);

  delay(100);

  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, HIGH);

  delay(100);
  
}

void dj_set() {

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

  delay(100);
}
