
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
 #include <DHT.h>



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
#define SensorPinDHT 20
#define SensorPinGirouette A9


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
DHT dht;
long DELAI = 1000;
int nbBasculement = 0;
int avant = 1;
int value = 0;
int e=0;
String paquet="";
//int DELAY = 10000; //2 secondes
long temps=0;
long ATTENTE = 2000;
float humidity = 0.0;
float temperature = 0.0;
long tempsDHT=0;
int i=0;

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
  Serial.begin(9600);
  initRF();
  anemometre.init();
  ec.init();
  dht.setup(SensorPinDHT);
  pinMode(SensorPinPluviometre, INPUT);
  tempsDHT=millis();
  temps=millis();
  
}

void loop(){
  updateSensor();
  //if(millis() - temps > DELAI){
    sendPaquet(getTrame(), ADDRESSE_RELAI);
    Serial.print("paquet ");
    Serial.print(i);
    Serial.println(" sent");
    i++;
    temps = millis();
  //}
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
  updateDHT();
}

void updateDHT(){
  if(millis() - tempsDHT > dht.getMinimumSamplingPeriod() ){
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
    tempsDHT = millis();
  }
}

String getTrame(){
  String trame="";
  trame+=String(ph.getMesure(),1);
  trame+=";";
  trame+=String(orp.getMesure(),1);
  trame+=";";
  trame+=String((float)ec.getMesure(),1);
  trame+=";";
  trame+=String(turbidity.getMesure(),1);
  trame+=";";
  trame+=String((float)ec.getTemperature(), 1);
  trame+=";";
  trame+=String((float)anemometre.getMesure(),1);
  trame+=";";
  trame+=String(getDirectionVent(),1);
  trame+=";";
  trame+=String(humidity,1);
  trame+=";";
  trame+=String(temperature,1);
  trame+=";";
  trame+=String((float)nbBasculement, 1);
  //Serial.println(nbBasculement);
  nbBasculement=0;
  //Serial.print("taille paquet =");
  Serial.println(trame);
  Serial.flush();
  return trame;
  
}

void getBasculement(){
  value = digitalRead(SensorPinPluviometre);
  if (value == 0 && avant == 1) 
    nbBasculement++;
  avant = value;
}


float getDirectionVent(){
  float valeur = analogRead(SensorPinGirouette)*(5.0/1023.0);
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


