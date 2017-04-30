
// Include the SX1272 and SPI library: 
#include "SX1276.h"
#include <Console.h>
#include <SPI.h>

int e;
String paquet="";

void setup()
{
  // Open Console communications and wait for port to open:
  Bridge.begin(115200);
  Console.begin();
  
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
  
  e = sx1276.receivePacketTimeout(1000);
  e = sx1276.getRSSIpacket();
 paquet = sx1276.getPacketRecu();
  
  
  Console.print(("Receive packet timeout, state "));
  Console.println(e, DEC);
  for(unsigned int i=0; i<sx1276._payloadlength; i++)
    Console.print((char)sx1276.packet_received.data[i]);
  Console.print("le paquet recu est : ");
  Console.println( paquet);
}

