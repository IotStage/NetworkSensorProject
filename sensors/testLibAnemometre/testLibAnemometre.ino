#include <WSNAnemometreSensor.h>

Anemometre a(3);


void setup() {
  Serial.begin(9600);
  a.init();
}

void loop() {
  a.updateAnemometre();
  Serial.println(a.getMesure());
}
