#include "SX1276.h"
#include <SPI.h>
#include <Wire.h>
#define ADDSERVER 8
#define ADDNODE 3
#define ADDWIRESLAVE 4


long temps =0;
long intervalle = 10000; //10 secondes

void initLora(){
  
  sx1276.ON(); 
  sx1276.setMode(1);
  sx1276.setChannel(CH_16_868);
  sx1276.setMaxCurrent(0x1B);
  sx1276.getMaxCurrent();
  sx1276.setPower('M');
  sx1276.setNodeAddress(ADDNODE);
  Serial.print("SX1276 successfully configured ");
}


void setup()
{
  Serial.begin(115200);
  initLora();
  Wire.begin();
  temps = millis();
}

void loop(void)
{
   if(millis() - temps > intervalle ){
      Wire.requestFrom(ADDWIRESLAVE, 32);
      String data = getDataFromWrite();
      if(data != "" ){
        sendDonnees(data);
      }
   }

   if(availablePacket()){
      String info = sx1276.getPacketRecu();
      if(){
        
      }
   }
  

}

String getDataFromWrite(){
  String data="";
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    data+=String(c);   // print the character
  }

  return data;
}


void sendDonnees(String paquet){
  Serial.println(paquet.length());
  char buff[paquet.length()+1];
  paquet.toCharArray(buff, paquet.length()+1); 
  sx1276.sendPacketTimeout(ADDSERVER, buff);
  Serial.print("Packet sent, state ");
  //Serial.println(e, DEC);
  Serial.println(strlen(buff));
  delay(5000);
}




boolean availablePacket(){
 
   sx1276.receivePacketTimeout(10000);
   String p = sx1276.getPacketRecu();
   if(e == 0 && p.length()>0){ 
    return true;
     //sendDonnees(paquet);
     //Serial.println("paquet envoye avec succes");
   }
   return false;
}
//*/




