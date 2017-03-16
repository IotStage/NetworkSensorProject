#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
#include "Timer.h"

Timer t;



void setup()
{
  Serial.begin(9600);
   if (!rf95.init())
    Serial.println("init failed");
 t.every(100, requestReceive);
  
}

void loop()
{
  t.update();
}

/*
void sendPacket(){
  uint8_t value[] = "mercie lora";
  rf95.send(value, sizeof(value));
  rf95.sleep();
  Serial.println("packet sent to client");
  //requestReceive();
}*/


void requestReceive(){
  if(rf95.available())
  {
    long times = millis();
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      Serial.print("got request: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      long afterTime= millis();
      Serial.print("temps pour le taitement = ");
      Serial.println(afterTime-times);
    
    }
    
  }
}






