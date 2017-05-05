#include <WSNAnemometreSensor.h>

Anemometre a(3);
const int pinPluvio = 5;
int nbBasculement = 0;
int avant = 1;
int value = 0;
long pause=0;

void setup() {
  Serial.begin(9600);
  a.init();
  pinMode(pinPluvio, INPUT);
  //t.every(5, getBasculement);
  pause = millis();
  
}

void loop() {
  // put your main code here, to run repeatedly:
   a.updateAnemometre();
  getBasculement();
  if(millis() - pause > 10)
  {
    Serial.print(nbBasculement);//delay(500);
    Serial.print(",");
    Serial.println(a.getMesure());
    nbBasculement=0;
    pause = millis();
  }
  //t.update();
  //delay(10);
  
}

void getBasculement() {
  //nbBasculement=0;
  value = digitalRead(pinPluvio);
  if (value == 0 && avant == 1) {
    nbBasculement++;}
  avant = value;
  //return nbBasculement;
}

