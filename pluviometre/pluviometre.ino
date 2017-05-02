#include "Timer.h"

Timer t;
const int pinPluvio = 8;
int nbBasculement = 0;
int avant = 1;
int value = 0;
long pause=0;

void setup() {
  Serial.begin(9600);
  pinMode(pinPluvio, INPUT);
  //t.every(5, getBasculement);
  pause = millis();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  getBasculement();
  if(millis() - pause > 100)
  {
    Serial.println(nbBasculement);//delay(500);
    nbBasculement=0;
    pause = millis();
  }
  //t.update();
  delay(10);
  
}

void getBasculement() {
  //nbBasculement=0;
  value = digitalRead(pinPluvio);
  //tension_pluvio=(sensorValue_pluvio*5)/1023;
  //Serial.print(value);
  //Serial.print(",");
  if (value == 0 && avant == 1) {
    nbBasculement++;}
  avant = value;
  //return nbBasculement;
}

