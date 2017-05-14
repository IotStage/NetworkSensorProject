#define BAUDRATE 115200

//If you use Dragino Yun Mesh Firmware , uncomment below lines. 
//#define BAUDRATE 250000

//#include <//Console.h>
#include <Bridge.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <HttpClient.h>
#include <Console.h>

HttpClient client;
#define ADDRESSE_GATEWAY 4
#define ADDRESSE_RELAI 9
//int HEART_LED=A2;

int e;
String paquet="";

RH_RF95 rf95;

int led = A2;
float frequency = 868.0;

void setup()
{
  // Open //Console communications and wait for port to open:
  ////Console.begin(115200);
  pinMode(led, OUTPUT);     
  Bridge.begin(BAUDRATE);
  //Console.begin();
  /*while (!//Console){
    digitalWrite(led, HIGH);   // turn the HEART_LED on (HIGH is the voltage level)
    delay(1000);              // wait for a se//Cond
    digitalWrite(led, LOW); 
    delay(1000);; // Wait for //Console port to be available
    //Console.begin();
  }*/
  //Console.println("Start Sketch");
  //Console.println("Debut initialisation RF");
  
 
  while (!Console) ; // Wait for //Console port to be available
  Console.println("Start Sketch");
  if (!rf95.init())
    //Console.println("init failed");
  // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);
  // Defaults BW Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  //Console.print("Listening on frequency: ");
  //Console.println(frequency);
  //Console.println(e, DEC);
  clignoterLED();
  Console.print("Configuration terminee");
}

void loop(void)
{

   if (rf95.available())
  {
    clignoterLED();
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      clignoterLED();
      //digitalWrite(led, HIGH);
      RH_RF95::printBuffer("request: ", buf, len);
      Console.print("got request: ");
      Console.println((char*)buf);
      Console.print("RSSI: ");
      Console.println(rf95.lastRssi(), DEC);
      String recu="";
      for(int i=0; i<50; i++){
        if((char)buf[i] >= ' ' && (char)buf[i]<='z' )
          recu+=(char)buf[i];
      }
      Console.print("data is ");
      Console.println(recu);
      clignoterLED();
      saveData(recu);
      clignoterLED();
      // Send a reply
      /*uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      //Console.println("Sent a reply");
      digitalWrite(led, LOW);*/
      //digitalWrite(led, LOW);
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  
  // Receive message
  //sendPaquet("paquet", ADDRESSE_RELAI);
  //delay(1000);
  //checkPacket();
  //delay(1000);
}

void saveData(String sensor){
  
  String url="http://10.130.1.200:8888/wsn/web/app_dev.php/pushDatas/";
  url+=sensor;
 client.get(url);

  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Console.print(c);
  }
  
  //Console.println("send data done.");
  Console.flush();
}

void clignoterLED(){
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  delay(50);
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  delay(50);
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  delay(10);
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
}

