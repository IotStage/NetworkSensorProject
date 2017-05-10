#include "SX1276.h"
#include <Console.h>
#include <SPI.h>


int e;
String paquet="";

void setup()
{
  // Open Console communications and wait for port to open:
  //Serial.begin(115200);
  Bridge.begin(115200);
  Console.begin();
  while (!Console) ; // Wait for console port to be available
  Console.println("Start Sketch");
  // Print a start message
  Console.println("SX1276 module and Arduino: receive packets without ACK");
  
  // Power ON the module
  sx1276.ON();
  
  // Set transmission mode and print the result
  e = sx1276.setMode(4);
  Console.println(e, DEC);
  
  // Select frequency channel
  e = sx1276.setChannel(CH_16_868);
  Console.println("Setting Channel: state ");
  Console.println(e, DEC);
  
  // Select output power (Max, High or Low)
  e = sx1276.setPower('M');
  Console.println("Setting Power: state ");
  Console.println(e);
  
  // Set the node address and print the result
  e = sx1276.setNodeAddress(9);
  Console.println(e, DEC);
  
  // Print a success message
  Console.print("SX1276 successfully configured ");
}

void loop(void)
{
  // Receive message
  String paquet = "paquet";
  sendPaquet(paquet, 3);
  checkPacket();
}

void sendPaquet(String paquet, int idClient){
  char buff[paquet.length()];
   paquet.toCharArray(buff, paquet.length()); 
  // Send message1 and print the result
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
  Console.print("le paquet recu est : ");
  Console.println(paquet);
  if(sx1276._payloadlength > 0){
   // String value = paquet.substring(5, paquet.length());
    //Console.println(value);  
    saveData(paquet);
  }
}

void saveData(String sensor){
  Process logdata;
  logdata.begin("python");
  logdata.addParameter("/root/datalogger.py");  //
  //logdata.addParameter("vitesse");
  logdata.addParameter(sensor);//
  Console.println(sensor);
  logdata.run();
 
  // read the output of the command
  while (logdata.available() > 0) {
    char c = logdata.read();
  }
//  logdata.stop();
  Console.println("send data done.");
}

