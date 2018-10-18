/**
 * test Capteur de Courant 
 * author : bassirou ngom
 * product : Current Sensor ACS711 20A
 */

const int pinOut = A0;
const int sensibilite = 100; //mmv/A
int offset = 2500.0; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
Serial.println(getCurrentValue());
  delay(1000);

}


float getCurrentValue(){

    float lu = analogRead(pinOut);
    float voltage = lu*(5000.0/1023.0); // + 15;
    float y = offset-voltage; //en mV
    
    float value = y/sensibilite;
  
    return value;
  
}
