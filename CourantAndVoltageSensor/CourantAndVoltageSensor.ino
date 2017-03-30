/**
 * test Capteur de tension et de courant 
 * author : bassirou ngom
 * product : Precision Voltage Sensor 1135
 */

 //constantes tension
const float sensibiliteTension = 0.0681*1000;
const float offsetTension = 2475.0;
const int voltageSensorPin = A0;

//constantes courant
const int pinOut = A1;
const int sensibiliteCourant = 100; //mmv/A
const int offsetCourant = 2500; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
   Serial.print(getVoltageValue());
   Serial.print(" ");
   Serial.println(getCurrentValue());
  delay(1000);

}

float getVoltageValue(){
  
  int sensor = analogRead(voltageSensorPin);
  
  float voltage = sensor*(5000.0/1023.0);
  
  float num = voltage - offsetTension;
  
  float mesure = num / sensibiliteTension;
  
  return mesure;
}

float getCurrentValue(){

    int lu = analogRead(pinOut);
    
    float voltage = lu*(5000.0/1023.0); // + 15;
    
    float y = offsetCourant-voltage; //en mV
    
    float value = y/sensibiliteCourant;
  
    return value;
  
}
