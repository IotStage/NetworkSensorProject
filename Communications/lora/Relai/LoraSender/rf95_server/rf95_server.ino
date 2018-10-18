// rf95_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_client
// Tested with Anarduino MiniWirelessLoRa

#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 5); // RX, TX
#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;
//int led = 8;
String command = ""; // Stores response of the HC-06 Bluetooth device
void setup() 
{
    //pinMode(led, OUTPUT);     
  Serial.begin(9600);
  if (!rf95.init())
    Serial.println("init failed");  
    mySerial.begin(9600);
  mySerial.write("AT+NAMEnode1");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
}

void loop()
{
  //uint8_t d[] = "test lora";
  if (mySerial.available()) {
    while(mySerial.available()) { // While there is more to be read, keep reading.
      command += String((char)mySerial.read());
    }
    
    Serial.println(command);
    command = ""; // No repeats
  }
  // Read user input if available.
  if (Serial.available()){
    delay(10); // The delay is necessary to get this working!
    mySerial.write(Serial.read());
  }
  //requestReceive();
  
  //sendPacket();
  delay(4000);
  
}

void sendPacket(){
  /*if(Serial.available() > 0 ){
    String t = Serial.readString();
    uint8_t value[t.length()];
    t.toCharArray(value, t.length());
    rf95.send(value, sizeof(value));
    rf95.waitPacketSent();
    Serial.println("packet sent");
  }*/
   //= "test lora";
  
  //delay(400);
}

void requestReceive(){
  if(rf95.available())
  {
    Serial.println("receive packet");
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

    
    uint8_t value[]= "merci boy";
    //t.toCharArray(value, t.length());
    rf95.send(value, sizeof(value));
    rf95.waitPacketSent();
    Serial.println("packet sent");
    //sendPacket();
  }else
    Serial.println("aucun packet recu");
}

