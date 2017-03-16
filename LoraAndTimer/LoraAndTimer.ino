#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
#include "Timer.h"

Timer t;

int ledEvent;

void setup()
{
  Serial.begin(9600);
  int tickEvent = t.every(2000, doSomething, 10);
  Serial.print("2 second tick started id=");
  Serial.println(tickEvent);
  
  pinMode(13, OUTPUT);
  ledEvent = t.oscillate(7, 50, HIGH);
  Serial.print("LED event started id=");
  Serial.println(ledEvent);
  
  int afterEvent = t.after(10000, doAfter);
  Serial.print("After event started id=");
  Serial.println(afterEvent); 
  
}

void loop()
{
  t.update();
}

void doSomething()
{
  Serial.print("2 second tick: millis()=");
  Serial.println(millis());
  
}


void doAfter()
{
  Serial.println("stop the led event");
  t.stop(ledEvent);
  t.oscillate(7, 500, HIGH, 1);
}


void sendPacket(){
  uint8_t value[] = "mercie lora";
  rf95.send(value, sizeof(value));
  rf95.sleep();
  //requestReceive();
}

void requestReceive(){
  while(rf95.available())
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
}






