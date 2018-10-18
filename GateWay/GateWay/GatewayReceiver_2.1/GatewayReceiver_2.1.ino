/*
 # Gateway receiver without relay 
 # Vers   : 3.0
 # Product: Dragino Gateway LG01
 # Author : Bassirou NGOM
 # email : bassiroungom26@gmail.com
 # Require: Bridge, ecrypt and SX1276 librairie 
 */

#define BAUDRATE 115200

//If you use Dragino Yun Mesh Firmware , uncomment below lines. 
//#define BAUDRATE 250000


#include <Bridge.h>
#include <SPI.h>
#include <SX1276.h>
#include <HttpClient.h>
#include <Console.h>
#include <String.h>
#include "encrypt.h"

HttpClient client;
#define ADDRESSE_GATEWAY 8
#define ADDRESSE_NODE 3
#define INFO 0
#define CTL 1
#define SUP 0
#define CMD 1

String ADDRESSE_SERVER ="196.1.95.67";
String PORT_SERVER ="8888";

//int HEART_LED=A2;

int e;
String paquet="";

RH_RF95 rf95;
ENCRYPT encrypt_decrypt;
unsigned char encryptkey[16]={0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF,0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};


int led = A2;
float frequency = 868.0;

void setup()
{
  // Open //Console communications and wait for port to open:
  ////Console.begin(115200);
  pinMode(led, OUTPUT);     
  Bridge.begin(BAUDRATE);
  Console.begin();
  
  while (!Console) ; // Wait for //Console port to be available
  Console.println("Start Sketch");
  initRF();
}

void loop(void)
{
  
 listenMsgEntrant();
 listenMsgSortant();
}

void saveData(String sensor, int type){
  String url="";
  if(type == INFO)
    url="http://"+ADDRESSE_SERVER+":"+PORT_SERVER+"/slim/pushDatas/";
  else
    url="http://"+ADDRESSE_SERVER+":"+PORT_SERVER+"/slim/putCTRL/";
  url+=sensor;
  Console.print("url=");
  Console.println(url);
  long debut = millis();
 client.get(url);

  while (client.available()) {
    char c = client.read();
    Console.print(c);
  }
  Console.print("send data done. ");
  Console.println(millis() - debut);
  Console.println("secondes");
  Console.flush();
}

void listenMsgEntrant(void){

   if(sx1276.availableData(100))
  {
    clignoterLED();
    sx1276.receivePacketTimeout(10000);
    Console.println("Reception d'un message");
    char buf[sx1276._payloadlength];//buffer to store the server response message
    uint8_t len = sx1276._payloadlength;// data length
    for(unsigned int i=0; i<sx1276._payloadlength; i++)
      buf[i]=(char)sx1276.packet_received.data[i];
    
    Console.println("decryptage du message "); 
    Console.println(buf);         
    encrypt_decrypt.btea(buf, -len, encryptkey);
    if(buf[0] == 'I' && buf[1] == 'N' && buf[2] == 'F' && buf[3] == ':' )
    {
      clignoterLED();
      char buff2[len-4];
      for(int i = 0; i<len-4; i++)
        buff2[i]=buf[i+4];
      String valeur = String(buff2);
      clignoterLED();
      saveData(valeur, INFO);
      clignoterLED();
    }
  }
}

void listenMsgSortant(){
  clignoterLED();
  String msg = getMsg(CMD);
  clignoterLED();
  if(msg != ""){
    clignoterLED();
    sendPaquet(msg, CMD);
    clignoterLED();
  }
}

String getMsg(int type){
  String msg = "";
   String url="";
  if(type == SUP)
    url="http://"+ADDRESSE_SERVER+"/plateforme/supervisionMsg";
  else
    url="http://"+ADDRESSE_SERVER+"/plateforme/cmdMsg";
 client.get(url);
 Console.println(url);
 clignoterLED();
  while (client.available()) {
    char c = client.read();
    //Console.print(c);
    msg+=c;
  }
  clignoterLED();
Console.print("le message sortant est :");
Console.println(msg);
clignoterLED();
  return msg;
}

void sendPaquet(String paquet, int type){
  int len = paquet.length()+1;
  Console.print("la taille du paquet est :");
  Console.println(len);
  char buff[len];
  paquet.toCharArray(buff, len);
  char buff2[len+4]; 
   if(type == SUP){
     buff2[0] = 'S';
     buff2[1] = 'U';
     buff2[2] = 'P';
     buff2[3] = ':';
   }else{
     buff2[0] = 'C';
     buff2[1] = 'M';
     buff2[2] = 'D';
     buff2[3] = ':';
   }
  for(int i=4; i<len+4; i++){
    buff2[i] = buff[i-4];
  }
  Console.print("la cmd est ");
  Console.println(buff2);
  encrypt_decrypt.btea(buff2, len, encryptkey);
  sx1276.sendPacketTimeout(ADDRESSE_NODE, buff2);
  //rf95.send(buff2, sizeof(buff2));
}


void clignoterLED(){
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  delay(50);
  
}



void initRF(){
  Serial.println(F("Debut initialisation RF"));
    // Power ON the module
  sx1276.ON();
  // Set transmission mode and print the result
  sx1276.setMode(1);
  sx1276.setChannel(CH_16_868);
  sx1276.setMaxCurrent(0x1B);
  sx1276.getMaxCurrent();
  sx1276.setNodeAddress(ADDRESSE_GATEWAY);
  // Select output power (Max, High or Low)
  /* H=13dBm M=17dBm  measured at the antenna connector*/
  sx1276.setPower('H');
  Serial.print("Configuration terminee");
}