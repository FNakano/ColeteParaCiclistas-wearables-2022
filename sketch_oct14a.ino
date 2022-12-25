#include <FastLED.h>

// MicNakano ***************************************************************
int MicSensorValue = 0;
const int MicMicPin = 13;
const int MicLedPin = 21;
int MicSensMin = 4095;
int MicSensMax = 0;
int MicLedState = LOW;
long MicPreviousMillis = 0;
int MicMillisOn = 0;

// YK04 ********************************************************************
const int RFRight = 18;
const int RFLeft = 19;
int RFLedState1 = LOW;
int RFLedState2 = LOW;
int RFLedState3 = LOW;
unsigned long RFPreviousMillis1 = 0;
unsigned long RFPreviousMillis2 = 0;
int RFMillisOn1 = 0;
int RFMillisOn2 = 0;

const long iterations = 1500;
const long blinkInterval = 100;

#define NUM_LEDS 24
#define NUM_LEDS2 4
#define DATA_PIN 4
#define DATA_PIN2 15
CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS2];


void led(int side, int state) { // Side: 0 = Right, 1 = Left. State: HIGH = On, LOW = Off
  byte i, j, r, g, b;

  if (side == 0) {
    i = 0; // Beginning of the strip
    j = NUM_LEDS / 2; // Half of the strip

    if (state == HIGH) {
      r = 255;
      g = 45;
      b = 0;

      RFLedState1 == HIGH;
    } else {
      r = 0;
      g = 0;
      b = 0;

      RFLedState1 == LOW;
    }
  } else {
    i = NUM_LEDS / 2; // Half of the strip
    j = NUM_LEDS; // End of the strip

    if (state == HIGH) {
      r = 255;
      g = 45;
      b = 0;

      RFLedState2 == HIGH;
    } else {
      r = 0;
      g = 0;
      b = 0;

      RFLedState2 == LOW;
    }
  }

  for (int cur = i; cur < j; cur++) {
    leds[cur] = CRGB(r, g, b);
  }

  FastLED.show();
}


void ledMic(int state) {
  byte i, j, r, g, b;

  i = 0;
  j = 4;

  if (state == HIGH) {
    r = 255;
    g = 45;
    b = 0;

    RFLedState3 == HIGH;
  } else {
    r = 0;
    g = 0;
    b = 0;

    RFLedState3 == LOW;
  }

  for (int cur = i; cur < j; cur++) {
    leds2[cur] = CRGB(r, g, b);
  }

  FastLED.show();
}


void setup() {
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN2>(leds2, NUM_LEDS2);

  pinMode(MicLedPin, OUTPUT);
  analogReadResolution(12);

  pinMode(RFRight, INPUT);
  pinMode(RFLeft, INPUT);
}

void loop() {
  // Mic ********************************************************************
  // read the analog / millivolts value for MicLedPin:
  MicSensorValue = analogRead(MicMicPin);
  
  MicSensMin = (MicSensorValue < MicSensMin) ? MicSensorValue : MicSensMin;
  MicSensMax = (MicSensorValue > MicSensMax) ? MicSensorValue : MicSensMax;

  if (MicSensorValue > 2300) {
    MicMillisOn = iterations;
  }

  if (MicMillisOn != 0) {
    unsigned long MicCurrentMillis = millis();

    if (MicCurrentMillis - MicPreviousMillis > blinkInterval) {
      // save the last time you blinked the LED
      MicPreviousMillis = MicCurrentMillis;
  
      // if the LED is off turn it on and vice-versa:
      if (MicLedState == LOW)
        MicLedState = HIGH;
      else
        MicLedState = LOW;
  
      // set the LED with the MicLedState of the variable:
      ledMic(MicLedState);
    }
    --MicMillisOn;
  } else {
    ledMic(LOW);
  }

  Serial.print(MicSensorValue);
  Serial.print(" ");
  Serial.print(MicSensMin);
  Serial.print(" ");
  Serial.println(MicSensMax);

  // Right RF ***************************************************************
  if(digitalRead(RFRight) > 0) {
    RFMillisOn1 = iterations;
  }

  Serial.println(RFMillisOn1);

  if (RFMillisOn1 > 0) {
    unsigned long RFcurrentMillis1 = millis();

    if (RFcurrentMillis1 - RFPreviousMillis1 >= blinkInterval) {
      // save the last time you blinked the LED
      RFPreviousMillis1 = RFcurrentMillis1;
  
      // if the LED is off turn it on and vice-versa:
      if (RFLedState1 == LOW)
        RFLedState1 = HIGH;
      else
        RFLedState1 = LOW;
  
      // set the LED with the MicLedState of the variable:
      led(0, RFLedState1);
    }
    --RFMillisOn1;
  } else {
    led(0, LOW);
  }

  // Left RF ****************************************************************
  if(digitalRead(RFLeft) > 0) {
    RFMillisOn2 = iterations;
  }

  Serial.println(RFMillisOn2);

  if (RFMillisOn2 > 0) {
    unsigned long RFcurrentMillis2 = millis();

    if (RFcurrentMillis2 - RFPreviousMillis2 > blinkInterval) {
      // save the last time you blinked the LED
      RFPreviousMillis2 = RFcurrentMillis2;
  
      // if the LED is off turn it on and vice-versa:
      if (RFLedState2 == LOW)
        RFLedState2 = HIGH;
      else
        RFLedState2 = LOW;
  
      // set the LED with the MicLedState of the variable:
      led(1, RFLedState2);
    }
    --RFMillisOn2;
  } else {
    led(1, LOW);
  }
}
