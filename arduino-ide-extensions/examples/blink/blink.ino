/* Test for the Devboard w/ atmega644 @ 11.059200 MHz */

const int ledPin = 15;

int val = 0;

void setup() {
  // Set the direction of the led pin
  pinMode(ledPin, OUTPUT);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(ledPin, LOW);
  delay(1000);
}
