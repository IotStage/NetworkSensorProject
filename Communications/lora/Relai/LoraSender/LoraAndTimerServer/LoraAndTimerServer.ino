#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
#include "Timer.h"

Timer t;
int idPacket = 0;



void setup()
{
  Serial.begin(9600);
   if (!rf95.init())
    Serial.println("init failed");
 t.every(5000, sendPacket);
  
}

void loop()
{
  t.update();
}


void sendPacket(){
  //Serial.println("function call");
  long times = millis();
  String dataString= "packet id = ";
  dataString+=idPacket;
  //uint8_t value[] = v.toCharArray();
  uint8_t dataArray[dataString.length()];
  dataString.toCharArray(dataArray, dataString.length());
  rf95.send(dataArray, sizeof(dataArray));
  rf95.sleep();
  Serial.print("packet sent to client");
  Serial.println(millis() - times);
  //requestReceive();
  idPacket++;
}

/*
void requestReceive(){
  if(rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      Serial.print("got request: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
    }
    
  }
}*/






