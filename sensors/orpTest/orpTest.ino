#include "WSNORPSensor.h"
ORP orp(A1);

long temps;
void setup() {
  Serial.begin(9600);
  temps = millis();
}

void loop() {
 orp.updateORP();
 if(millis() - temps > 100){
  Serial.println(orp.getMesure());
  temps=millis();
 }
 
}
