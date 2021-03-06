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

 Power:
 The board is powered by 5V outlet of the arduino

 Button:
 The button is wired to port #2 of the arduino and also has a 10k Ohm
 resistor bridge to ground (so that it gets recognized as LOW when not
 pressed)

 Break LED:
 The break LED indicates stand-by status (i.e. waiting for button press)
 It is wired to port 3 and ground with a 220 Ohm resistor

 Minute interval indicators:
 The 5 minute interval indicators are wired to ports 4-8 and each use a
 220 Ohm resistor.

 See layout.JPG for an overview
*/

const int firstStatusLED = 4;
const int LEDCount = 5;
const int breakLED = 3;
const int button = 2;


// setup

void setup() {
  // declare the LED pins as outputs
  pinMode(breakLED, OUTPUT);
  for(int i = 0; i < LEDCount; i++) {
    pinMode(led(i), OUTPUT);
  }

  // declare the switch pin as an input
  pinMode(button, INPUT);
  digitalWrite(breakLED, HIGH);
}


// helper methods

int led(int index) {
  return firstStatusLED + index;
}

void on(int index) {
  digitalWrite(led(index), HIGH);
}

void off(int index) {
  digitalWrite(led(index), LOW);
}

void reset() {
  digitalWrite(breakLED, HIGH);
  for(int i = 0; i < LEDCount; i++) {
    off(i);
  }
}


// pomodoro cycle loop

void pomodoroCycle() {
  digitalWrite(breakLED, LOW);

  for(int i = 0; i < LEDCount; i++) {
    on(i);
    for (int j = 0; j < LEDCount; j++) {
      delay(60000);
      if (i == LEDCount - 1) {
        off(j);
      }
    }
  }
}


// eye candy

void blinkLEDs(int times) {
  // light show, yay!
  for(int i = 0; i < times; i++) {
    for(int i = 0; i < LEDCount; i++) {
      on(i);
    }
    delay(260);
    for(int i = 0; i < LEDCount; i++) {
      off(i);
    }
    delay(260);
  }
  for(int i = 0; i < LEDCount; i++) {
    on(i);
  }
}

void cascadeLEDs(bool inverseDirection, int sig) {
  int dir = inverseDirection ? -1 : 1;
  int currentLED = 0;
  for(int i = 0; i < LEDCount; i++) {
    currentLED = led(inverseDirection ? LEDCount -i -1 : i);
    digitalWrite(currentLED, sig);
    delay(130);
  }
}

void knightRiderLEDs(bool inverseDirection, int delayTime) {
  int dir = inverseDirection ? -1 : 1;
  int currentLED = 0;
  int previousLED = -1;
  for(int i = 0; i < LEDCount; i++) {
    currentLED = inverseDirection ? LEDCount -i -1 : i;
    on(currentLED);
    if (previousLED >= 0) {
      off(previousLED);
    }
    previousLED = currentLED;
    delay(delayTime);
  }
  off(previousLED);
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
  knightRiderLEDLoop(30, false);
  blinkLEDs(3);
  cascadeLEDs(true, LOW);
}


// main app loop

void loop() {
  if (digitalRead(button) == HIGH) {
    pomodoroCycle();
    lightShow();
    reset();
  }
}
