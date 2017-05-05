#include "SX1276.h"
#include "Timer.h"
#include <SPI.h>
#define ADDSERVER 8
#define ADDNODE 3

Timer t;
//variables Lora
int e;

//variables anemometre
unsigned long tempsDebut=0;
long nbTour=0;
boolean etatActuel;
const int pingAnemometre = 5;

//variable girouette
const int pinGirouette = A0;

//variables pluviometrie
const int pinPluvio = 3;
int nbBasculement=0;
int avant=1;
int value=0;

void initLora(){
  // Serial.println("SX1276 module and Arduino: send packets without ACK");
  sx1276.ON(); 
  e = sx1276.setMode(1);
  //Serial.println(e, DEC);
  e = sx1276.setChannel(CH_16_868);
  //Serial.println("Setting Channel: state ");
  //Serial.println(e, DEC);
  sx1276.setMaxCurrent(0x1B);
  sx1276.getMaxCurrent();
  // Select output power (Max, High or Low)
  /* H=13dBm M=17dBm  measured at the antenna connector*/
  e = sx1276.setPower('M');
  //Serial.println("Setting Power: state ");
  //Serial.println(e);
  e = sx1276.setNodeAddress(ADDNODE);
  //Serial.println(e, DEC);
  // Print a success message
  Serial.print("SX1276 successfully configured ");
}


void setup()
{
  Serial.begin(115200);
  initLora();
  pinMode(pingAnemometre, INPUT);
  pinMode(pinPluvio, INPUT);
 // t.every(100, checkPacket);
  //t.every(10, getBasculement);
}

void loop(void)
{
  
  //Serial.println(formatagePaquet());
  
  //t.update();
  //checkPacket();
 String paquet = formatagePaquet();
  Serial.print(" trame = ");
  Serial.println(paquet);
  sendDonnees(paquet);
  Serial.println("paquet envoye avec succes");
}

int getNBTour(){
  int nb=0;
  etatActuel = digitalRead(pingAnemometre);
  if(etatActuel==1 ){
    //nbTour=0;
    tempsDebut=millis();
    while(millis()-tempsDebut<1000){
      int etat = digitalRead(5);
      nb = (etatActuel !=  etat) ? nb+1 : nb; 
      etatActuel = etat;   
      
    }
    nbTour=nb;
  }else{
    nb=nbTour;
  }
  return nb;
}

void sendDonnees(String paquet){
  Serial.println(paquet.length());
  char buff[paquet.length()+1];
   paquet.toCharArray(buff, paquet.length()+1); 
  e = sx1276.sendPacketTimeout(ADDSERVER, buff);
  Serial.print("Packet sent, state ");
  Serial.println(e, DEC);
  Serial.println(strlen(buff));
  delay(5000);
}

String getVistesseVent(){
  String paquet="";
  paquet+=2*3.14*7.2*0.01*getNBTour();
  return paquet;
}

float getDirectionVent(){
  float valeur = analogRead(pinGirouette)*(5.0/1023.0);
  if(valeur<=0.32)
    return 112.5;
  else if(valeur <= 0.41)
    return 67.5;
  else if(valeur <= 0.45)
    return 90.0;
  else if(valeur <= 0.62)
    return 157.5; 
  else if(valeur <= 0.90)
    return 135.0;
  else if(valeur <= 1.19)
    return 202.5;
  else if(valeur <= 1.40)
    return 180.0;
  else if(valeur <= 1.98)
    return 22.5;
  else if(valeur <= 2.25)
    return 45.0;
  else if(valeur <= 2.93)
    return 147.5;
  else if(valeur <= 3.08)
    return 225.0;
  else if(valeur <= 3.43)
    return 137.5;
  else if(valeur <= 3.84)
    return 0.0;
  else if(valeur <= 4.04)
    return 292.5;
  else if(valeur <= 4.62)
    return 270.0;
  else if(valeur <= 4.78)
    return 315.0;
}
int getBasculement(){
  long temps=millis();
  nbBasculement=0;
  while(millis() - temps < 1000){
    value = digitalRead(pinPluvio);
  //Serial.print(value);Serial.print(",");
  //Serial.println(nbBasculement);
//Serial.print(",");
  if(value==0 && avant==1){
    nbBasculement++;
  }
  avant=value;
  }
   
  return nbBasculement;
}

String formatagePaquet(){
  String  paquet = String(getVistesseVent());
  paquet+=" ";
  paquet+=String(getDirectionVent());
  paquet+=" ";
  paquet+=String(getBasculement());
  paquet+=" ";
  paquet+="fin";
  //nbBasculement=0;
  return paquet;
}

/*void checkPacket(){
 
   e = sx1276.receivePacketTimeout(10000);
   String p = sx1276.getPacketRecu();
   if(e == 0 && p.length()>0){ 
     sendDonnees(paquet);
     Serial.println("paquet envoye avec succes");
   }
}
*/




