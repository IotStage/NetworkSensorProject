// rf95_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_server
// Tested with Anarduino MiniWirelessLoRa

#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;

void setup() 
{
  Serial.begin(9600);
  if (!rf95.init()){
    Serial.println("init failed");
    return;
  }
    
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

}

void loop()
{
  //requestReceive();
  //delay(2000);
  //uint8_t d[] = "merci lora";
  sendPacket();
}

void sendPacket(){
  uint8_t value[] = "mercie lora";
  rf95.send(value, sizeof(value));
  rf95.sleep();
  requestReceive();
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



