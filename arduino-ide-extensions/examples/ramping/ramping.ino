/* Test for the Devboard w/ atmega644 @ 11.059200 MHz */

const int ledPin = 15;

int val = 0;

void setup() {
  // Set the direction of the led pin
  pinMode(ledPin, OUTPUT);
}

void loop() {
  for(val = 0; val < 255; val++)
  {
    analogWrite(ledPin, val);
    delay(4);
  }
  for(val = 255; val > 0; val--)
  {
    analogWrite(ledPin, val);
    delay(4);
  }
}
