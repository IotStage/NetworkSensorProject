#include "SX1276.h"
#include <Console.h>
#include <SPI.h>

#define ADDRESSE_GATEWAY 4
#define ADDRESSE_RELAI 9
int HEART_LED=A2;

int e;
String paquet="";

void setup()
{
  // Open Console communications and wait for port to open:
  //Console.begin(115200);
  pinMode(HEART_LED, OUTPUT);
  Bridge.begin(115200);
  Console.begin();
  while (!Console){
    digitalWrite(HEART_LED, HIGH);   // turn the HEART_LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(HEART_LED, LOW); 
    delay(1000);; // Wait for console port to be available
  }
  Console.println("Start Sketch");
  Console.println("Debut initialisation RF");
  
  sx1276.ON();
  e = sx1276.setMode(4);
  Console.print("configuration du mode de transmission ");
  Console.println(e, DEC);
  
  e = sx1276.setChannel(CH_11_868);
 Console.print("configuration du canal de transmission ");
  Console.println(e, DEC);
  
  e = sx1276.setPower('M');
  Console.print("configuration de la puissance ");
  Console.println(e);
  
  
  e = sx1276.setNodeAddress(ADDRESSE_GATEWAY);
  Console.print("configuration de l'adresse du relai ");
  Console.println(e, DEC);
  
  Console.print("Configuration terminee");
}

void loop(void)
{
  
  // Receive message
  //sendPaquet("paquet", ADDRESSE_RELAI);
  //delay(1000);
  checkPacket();
  delay(1000);
}

void sendPaquet(String paquet, int idClient){
  char buff[paquet.length()];
   paquet.toCharArray(buff, paquet.length()); 
  e = sx1276.sendPacketTimeout(idClient, buff);
  Console.print("Packet sent, state ");
  Console.println(e, DEC);
  Console.println(buff);
  
}

void checkPacket(){
  e = sx1276.receivePacketTimeout(10000);
  e = sx1276.getRSSIpacket();
  paquet = sx1276.getPacketRecu();
  Console.print(("Receive packet timeout, state "));
  Console.println(e, DEC);
 Console.println(sx1276._payloadlength);
  if(sx1276._payloadlength > 0){
   // String value = paquet.substring(5, paquet.length());
    //Console.println(value);  
     Console.print("le paquet recu est : ");
    Console.println(paquet);
       // turn the HEART_LED off by making the voltage LOW
    //delay(1000);
    saveData(paquet);
  }
}

void saveData(String sensor){
  Process logdata;
  logdata.begin("python");
  logdata.addParameter("/root/datalogger.py");  //
  //logdata.addParameter("vitesse");
  logdata.addParameter(sensor);//
  //Console.println(sensor);
  logdata.run();
 
  // read the output of the command
 /* while (logdata.available() > 0) {
    char c = logdata.read();
    Console.print(c);
  }*/
 // logdata.stop();
  Console.println("send data done.");
}

