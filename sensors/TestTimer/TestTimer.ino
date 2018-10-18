#include "Timer.h"

Timer t;
int pin = 7;

void setup()
{
  Serial.begin(9600);
  pinMode(pin, OUTPUT);
  t.oscillate(pin, 1000, 1);
  t.every(100, takeReading);
}

void loop()
{
  t.update();
}

void takeReading()
{
  Serial.println(analogRead(0));
}
