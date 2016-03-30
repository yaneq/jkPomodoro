/*
  jkPomodoro

 Parts required:
 1 green LED
 5 red LEDs
 pushbutton
 10 kilohm resistor
 5 220 ohm resistors

 Created 30th of March 2016
 by Jan Koeppen

 The setup consists of 5 red LEDs and a green LED.
 Initially, the green light is on to indicate break-mode. After
 triggering the button, the red LEDs will turn on one-by-one
 in 5 minute intervals, until 25 minutes have passed. To signalize
 the finalization of the pomodoro cycle, all 5 LEDs will blink
 several times. The system will go back to the initial status
 with only the green LED turned on and a new cycle can be started.

*/

const int firstStatusLED = 4;
const int LEDCount = 5;
const int breakLED = 3;
const int button = 2;

void setup() {
  // declare the LED pins as outputs
  pinMode(breakLED, OUTPUT);
  for(int i = 0; i < LEDCount; i++) {
    pinMode(firstStatusLED + i, OUTPUT);
  }

  // declare the switch pin as an input
  pinMode(button, INPUT);
  digitalWrite(breakLED, HIGH);
}

int getLEDID(int index) {
  return firstStatusLED + index;
}

// eye candy

void blinkLEDs(int times) {
  // light show, yay!
  for(int i = 0; i < times; i++) {
    for(int i = 0; i < LEDCount; i++) {
      digitalWrite(firstStatusLED + i, HIGH);
    }
    delay(260);
    for(int i = 0; i < LEDCount; i++) {
      digitalWrite(firstStatusLED + i, LOW);
    }
    delay(260);
  }
  for(int i = 0; i < LEDCount; i++) {
    digitalWrite(firstStatusLED + i, HIGH);
  }
}

void cascadeLEDs(bool inverseDirection, int sig) {
  int dir = inverseDirection ? -1 : 1;
  int currentLED = 0;
  for(int i = 0; i < LEDCount; i++) {
    currentLED = getLEDID(inverseDirection ? LEDCount -i -1 : i);
    digitalWrite(currentLED, sig);
    delay(130);
  }
}

void knightRiderLEDs(bool inverseDirection, int delayTime) {
  int dir = inverseDirection ? -1 : 1;
  int currentLED = 0;
  int previousLED = -1;
  for(int i = 0; i < LEDCount; i++) {
    currentLED = getLEDID(inverseDirection ? LEDCount -i -1 : i);
    digitalWrite(currentLED, HIGH);
    if (previousLED >= 0) {
      digitalWrite(previousLED, LOW);
    }
    previousLED = currentLED;
    delay(delayTime);
  }
  digitalWrite(previousLED, LOW);
}

void knightRiderLEDLoop(int iterations, bool inverse) {
  int initSpeed = 130;
  int speed = 0;
  int mult = 0;
  for (int i = 1; i < iterations; i++) {
    mult = inverse ? iterations - i : i;
    speed = floor(1/log(mult*0.7+1)*60);
    knightRiderLEDs(false, speed);
    knightRiderLEDs(true, speed);
  }
}

void lightShow() {
  blinkLEDs(16);
  cascadeLEDs(false, LOW);
  cascadeLEDs(false, HIGH);
  cascadeLEDs(false, LOW);
  cascadeLEDs(false, HIGH);
  cascadeLEDs(false, LOW);
  knightRiderLEDs(true,130);
  knightRiderLEDs(false,130);
  cascadeLEDs(true, HIGH);
  cascadeLEDs(true, LOW);
  cascadeLEDs(false, HIGH);
  blinkLEDs(2);
  knightRiderLEDLoop(20, false);
  cascadeLEDs(false, LOW);
  cascadeLEDs(true, HIGH);
  cascadeLEDs(true, LOW);
  cascadeLEDs(true, HIGH);
  blinkLEDs(3);
  cascadeLEDs(true, LOW);
}

void pomodoroCycle() {
  digitalWrite(breakLED, LOW);

  for(int i = 0; i < LEDCount; i++) {
    digitalWrite(firstStatusLED + i, HIGH);
    delay(300000);
  }
}

void reset() {
  digitalWrite(breakLED, HIGH);
  for(int i = 0; i < LEDCount; i++) {
    digitalWrite(firstStatusLED + i, LOW);
  }
}

// main app loop
void loop() {
  if (digitalRead(button) == HIGH) {
    pomodoroCycle();
    lightShow();
    reset();
  }
}

