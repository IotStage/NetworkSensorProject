#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>



// parametres shield ethernet
//IPAddress ip(172,16,0,101);
IPAddress ip(10,2,1,11);
EthernetClient client;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server [] = "10.2.1.10";

int taille=2;

#define SLAVE_RF 5
#define SLAVE_SMS 3
//#define TAILLERF 25
#define TAILLE 25


void setup() {
 Serial.begin(9600);
 Wire.begin();

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



