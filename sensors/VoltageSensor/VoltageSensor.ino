/**
 * test Capteur de tension 
 * author : bassirou ngom
 * product : Precision Voltage Sensor 1135
 */
const float sensibilite = 0.0681*1000;
const float offset = 2475.0;
const int voltageSensorPin = A0;

 

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(getVoltageValue());
  delay(1000);
}

float getVoltageValue(){
  int sensor = analogRead(voltageSensorPin);
  float voltage = sensor*(5000.0/1023.0);
  float num = voltage - offset;
  float mesure = num / sensibilite;
  return mesure;
}




