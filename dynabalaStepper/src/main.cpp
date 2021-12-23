#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(PB7, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PB7, HIGH);
  delay(1000);
  digitalWrite(PB7, LOW);
  delay(1000);
}