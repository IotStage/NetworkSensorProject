/*
 *  Semtech SX1272 module managing with Arduino
 *
 *  Copyright (C) 2014 Libelium Comunicaciones Distribuidas S.L.
 *  http://www.libelium.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 2.1 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.

 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Version:    1.0
 *  Design:   David Gascón
 *  Implementation: Covadonga Albiñana
 */
 
// Include the SX1272 and SPI library: 
#include "SX1276.h"
#include <Console.h>
#include <SPI.h>

int e;
String paquet="";

void setup()
{
  // Open Console communications and wait for port to open:
  Serial.begin(115200);
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
  Serial.print("Packet sent, state ");
  Serial.println(e, DEC);
  Serial.println(buff);
  
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
    String value = paquet.substring(5, paquet.length());
    //Console.println(value);  
    saveData(value);
  }
}

void saveData(String sensor){
  Process logdata;
  // date is a command line utility to get the date and the time
  // in different formats depending on the additional parameter
  logdata.begin("python");
  logdata.addParameter("/root/datalogger.py");  //
  logdata.addParameter("vitesse");
  logdata.addParameter(sensor);//
  logdata.run();  // run the command
 
  // read the output of the command
  while (logdata.available() > 0) {
    char c = logdata.read();
  }
  Console.println("send data done.");
}

