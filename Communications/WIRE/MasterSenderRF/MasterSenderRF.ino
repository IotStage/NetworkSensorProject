#include <WSNRFModule.h>
#include <Wire.h>
RFModule rf;
long debutTransmission;
long finTransmission;
String msg="";

void setup() {
  
  Serial.begin(38400);
  Serial.println("debut initialisation");
   Wire.begin();  
   Serial.println("fin initialisation wire");
  rf.initRF();
  Serial.println("fin initialisation rf");
  debutTransmission = millis();
  finTransmission = 1000;
  Serial.println("fin initialisation");
  
}

void loop() {
   long tmp = millis();
   msg="";
  //if(tmp - debutTransmission > finTransmission){
    Wire.requestFrom(8, 30);    // request 6 bytes from slave device #8
      Serial.println("juste ici ca marche");
    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character
      msg+=c;         // print the character
      
     }
     Serial.println("juste ici2 ca marche");
      Serial.println(msg);
      debutTransmission = tmp;
      rf.sendData(msg);
      Serial.println("paquet envoye avec succes");
  //}
  delay(finTransmission);
}
