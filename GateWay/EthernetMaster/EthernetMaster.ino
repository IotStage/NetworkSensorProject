#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>

#define SLAVE_RF 5
#define SLAVE_SMS 3
//#define TAILLERF 25
#define TAILLE 25


void setup() {
 Serial.begin();

}

void loop() {
  
}

void getDataFromRF(){
  String msg="";
  Wire.requestFrom(SLAVE_RF, TAILLE);
  while (Wire.available()) { 
      char c = Wire.read(); 
      msg+=c;
   }
  
}



void getDataFromSMS(){
   String msg="";
  Wire.requestFrom(SLAVE_RF, TAILLE, false);
  while (Wire.available()) { 
      char c = Wire.read(); 
      msg+=c;
  }

  Wire.requestFrom(SLAVE_RF, TAILLE);
  while (Wire.available()) { 
      char c = Wire.read(); 
      msg+=c;
  }
  
}



