
// Include the SX1272 and SPI library: 
#include "SX1276.h"
#include <SPI.h>

int e;
String paquet="";

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  
  // Print a start message
  Serial.println("SX1276 module and Arduino: receive packets without ACK");
  
  // Power ON the module
  sx1276.ON();
  
  // Set transmission mode and print the result
  e = sx1276.setMode(1);
  Serial.println(e, DEC);
  
  // Select frequency channel
  e = sx1276.setChannel(CH_16_868);
  Serial.println("Setting Channel: state ");
  Serial.println(e, DEC);
  
  // Select output power (Max, High or Low)
  e = sx1276.setPower('M');
  Serial.println("Setting Power: state ");
  Serial.println(e);
  
  // Set the node address and print the result
  e = sx1276.setNodeAddress(8);
  Serial.println(e, DEC);
  
  // Print a success message
  Serial.print("SX1276 successfully configured ");
}

void loop(void)
{
  // Receive message
  
  e = sx1276.receivePacketTimeout(10000);
  e = sx1276.getRSSIpacket();
 paquet = sx1276.getPacketRecu();
  
  
  Serial.print(("Receive packet timeout, state "));
  Serial.println(e, DEC);
  //for(unsigned int i=0; i<sx1276._payloadlength; i++)
  //  Serial.print((char)sx1276.packet_received.data[i]);
  Serial.print("le paquet recu est : ");
  Serial.println(paquet);
}

