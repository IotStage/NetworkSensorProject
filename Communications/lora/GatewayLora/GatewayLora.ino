#include "SX1276.h"
#include <Console.h>
#include <SPI.h>

int e;
void initLora(){
   Console.println("SX1276 module and Arduino: receive packets without ACK");
  
  // Power ON the module
  sx1276.ON();
  
  // Set transmission mode and print the result
  e = sx1276.setMode(1);
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
  e = sx1276.setNodeAddress(8);
  Console.println(e, DEC);
  
  // Print a success message
  Console.print("SX1276 successfully configured ");
}
void setup()
{
  // Open Console communications and wait for port to open:
  Bridge.begin(115200);
  Console.begin();
  initLora();
  // Print a start message
 
}

void loop(void)
{
  e = sx1276.receivePacketTimeout(10000);
  e = sx1276.getRSSIpacket();
  String paquet = sx1276.getPacketRecu();
  Console.print(("Receive packet timeout, state "));
  Console.println(e, DEC);
  Console.println(paquet);
  saveData(paquet);
}

void saveData(String sensor){
  Process logdata;
  logdata.begin("python");
  logdata.addParameter("/root/datalogger.py");  //
  //logdata.addParameter("vitesse");
  logdata.addParameter(sensor);//
  Console.println(sensor);
  logdata.run();
  delay(2000);
  // read the output of the command
  while (logdata.available() > 0) {
    char c = logdata.read();
  }
  Console.println("send data done.");
}

