
/*
 # Cet code rassemble tous les capteurs
 # Ver   : 2.1
 # Product: analog pH meter, turbidity, ORP, EC Meter, Temperature, anemometre, rain jauge
 # Author : Bassirou NGOM
 # email : bassiroungom26@gmail.com
 # Require: WSN Librairie and SX1276 librairie
 */
/************
 * inclusions
 ************/
 #include <WSNPHSensor.h>
 #include <WSNORPSensor.h>
 #include <WSNECSensor.h>
 #include <WSNTemperatureSensor.h>
 #include <WSNAnemometreSensor.h>
 #include <WSNTurbiditySensor.h>
 #include <SX1276.h>
  #include <SPI.h>



/************
 * Constantes
 ************/
#define SensorPinPH A0
#define SensorPinEC A1
#define SensorPinTurbidity A3
#define SensorPinORP A4
#define SensorPinTemperature 5
#define SensorPinAnemometre 3
#define SensorPinPluviometre 7


#define ADDRESSE_NODE 3
#define ADDRESSE_RELAI 7
#define DEBUG 0


/*************
 * Variables *
 *************/
Ph ph(SensorPinPH);
EC ec(SensorPinEC, SensorPinTemperature);
ORP orp(SensorPinORP);
Anemometre anemometre(SensorPinAnemometre);
Turbidity turbidity(SensorPinTurbidity);
int DELAI = 1000;
int nbBasculement = 0;
int avant = 1;
int value = 0;
int e=0;
String paquet="";
int DELAY = 2000; //2 secondes
long temps=0;
long ATTENTE = 2000;


/*************
 * fonctions *
 *************/
 void updateSensor();
 void sendDataToMaster();
 void getDataFromMaster();
 String getTrame();
 void getBasculement();
 void initRf();
 boolean availablePack();
 void sendPaquet(String paquet, int idClient);

/****************************
 * definition des fonctions *
 ****************************/
void setup(){
  Serial.begin(115200);
  initRF();
  anemometre.init();
  ec.init();
  pinMode(SensorPinPluviometre, INPUT);
  
}

void loop(){
  updateSensor();
  
  sendPaquet(getTrame(), ADDRESSE_RELAI);
  Serial.println("paquet sent");
  delay(DELAI);
  
}

void initRF(){
  Serial.println("Debut initialisation RF");

  sx1276.ON();
  e = sx1276.setMode(1);
  #if (DEBUG > 0)
    Serial.print("configuration du mode de transmission ");
    Serial.println(e, DEC);
  #endif
  e = sx1276.setChannel(CH_16_868);
  #if (DEBUG > 0)
    Serial.print("configuration du canal de transmission ");
    Serial.println(e, DEC);
  #endif
  e = sx1276.setPower('M');
  #if (DEBUG > 0)
    Serial.print("configuration de la puissance ");
    Serial.println(e);
  #endif
  
  e = sx1276.setNodeAddress(ADDRESSE_NODE);
  #if (DEBUG > 0)
    Serial.print("configuration de l'adresse du relai ");
    Serial.println(e, DEC);
  #endif
  Serial.print("Configuration terminee");
}

boolean availablePacket(){
  e = sx1276.receivePacketTimeout(10000);
  e = sx1276.getRSSIpacket();
  paquet = sx1276.getPacketRecu();
  #if (DEBUG > 0)
    Serial.print(("Receive packet timeout, state "));
    Serial.println(e, DEC);
    Serial.print("le paquet recu est : ");
    Serial.println(paquet);
  #endif
  if(sx1276._payloadlength > 0){
   return true;
  }
  return false;
}

void sendPaquet(String paquet, int idClient){
  char buff[paquet.length()+1];
  paquet.toCharArray(buff, paquet.length()+1); 
  e = sx1276.sendPacketTimeout(idClient, buff);
  #if (DEBUG > 0)
    Serial.print("Packet sent, state ");
    Serial.println(e, DEC);
    Serial.println(buff);
  #endif
  
}


void updateSensor(){
  ph.updatePh();
  ec.updateEC();
  orp.updateORP();
  anemometre.updateAnemometre();
  turbidity.updateTurbidity();
  getBasculement();
}

String getTrame(){
  String trame="";
  trame+="ph="+String(ph.getMesure(),2);
  //trame+=";";
  trame+=";or="+String(orp.getMesure(),2);
  //trame+=";";
  trame+=";ec="+String(ec.getMesure(),2);
  //trame+=";";
  trame+=";te="+String(ec.getTemperature(), 2);
  //trame+=";";
  trame+=";vi="+String(anemometre.getMesure(),2);
  //trame+=";";
  trame+=";tu="+String(turbidity.getMesure(),2);
  //trame+=";";
  //trame+=";pu="+nbBasculement;
  Serial.println(nbBasculement);
  nbBasculement=0;
  Serial.print("taille paquet =");
  Serial.println(trame.length());
  return trame;
  
}

void getBasculement(){
  //value = digitalRead(SensorPinPluviometre);
  //if (value == 0 && avant == 1) 
    nbBasculement++;
  avant = value;
}

