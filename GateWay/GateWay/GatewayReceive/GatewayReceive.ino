#include "SX1276.h"
#include <Console.h>
#include <SPI.h>
#include <HttpClient.h>


HttpClient client;

#define ADDRESSE_GATEWAY 8
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
  e = sx1276.setMode(1);
  Console.print("configuration du mode de transmission ");
  Console.println(e, DEC);
  
  e = sx1276.setChannel(CH_16_868);
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
  //delay(10000);
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
  //paquet = sx1276.getPacketRecu();
  for(unsigned int i=0; i<sx1276._payloadlength; i++)
    paquet+=(char)sx1276.packet_received.data[i];
  Console.println(sx1276._payloadlength);
  if(sx1276._payloadlength > 0){
    Console.print("le paquet recu est : ");
    Console.println(paquet);
    saveData(paquet);paquet="";
    
    
  }
}

void saveData(String sensor){
  
  String url="http://196.1.95.67/test/test.php?donnees=";
  url+=sensor;
  Console.println(url);
 client.get(url);

  /*while (client.available()) {
    char c = client.read();
    Console.print(c);
  }*/
  
  Console.println("send data done.");
  Console.flush();
}

