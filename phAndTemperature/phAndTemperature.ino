/*
 # Cette code permet de recuper la valeur du ph et le votage correspondant 
 # Ver   : 1.0
 # Product: analog pH meter
 # SKU   : SEN0169
 # Author : Bassirou NGOM 
*/
#include <OneWire.h>

#define SensorPin A0  //le pin ou est branche le capteur
#define Offset 0.00  //compensation de la deviation
#define LED 13      //le pin du led sur la carte
#define intervalMesure 20 //intervalle de ou on lit les valeurs sur le pin SensorPin
#define intervalConvertion 800 //intervalle ou les valeurs prises seront convertis en ph
#define tailleEchantillon  40  //taille des echantillons
int tableauPh[tailleEchantillon];  //tableau stokage des echantillons
int indexPh=0; 

/* Broche du bus 1-Wire */
const byte BROCHE_ONEWIRE = 2;

/* Code de retour de la fonction getTemperature() */
enum DS18B20_RCODES {
  READ_OK,  // Lecture ok
  NO_SENSOR_FOUND,  // Pas de capteur
  INVALID_ADDRESS,  // Adresse reçue invalide
  INVALID_SENSOR  // Capteur invalide (pas un DS18B20)
};


/* Création de l'objet OneWire pour manipuler le bus 1-Wire */
OneWire ds(BROCHE_ONEWIRE);


/**
 * Fonction de lecture de la température via un capteur DS18B20.
 */
byte getTemperature(float *temperature, byte reset_search) {
  byte data[9], addr[8];
  // data[] : Données lues depuis le scratchpad
  // addr[] : Adresse du module 1-Wire détecté
  
  /* Reset le bus 1-Wire ci nécessaire (requis pour la lecture du premier capteur) */
  if (reset_search) {
    ds.reset_search();
  }
 
  /* Recherche le prochain capteur 1-Wire disponible */
  if (!ds.search(addr)) {
    // Pas de capteur
    return NO_SENSOR_FOUND;
  }
  
  /* Vérifie que l'adresse a été correctement reçue */
  if (OneWire::crc8(addr, 7) != addr[7]) {
    // Adresse invalide
    return INVALID_ADDRESS;
  }
 
  /* Vérifie qu'il s'agit bien d'un DS18B20 */
  if (addr[0] != 0x28) {
    // Mauvais type de capteur
    return INVALID_SENSOR;
  }
 
  /* Reset le bus 1-Wire et sélectionne le capteur */
  ds.reset();
  ds.select(addr);
  
  /* Lance une prise de mesure de température et attend la fin de la mesure */
  ds.write(0x44, 1);
  delay(800);
  
  /* Reset le bus 1-Wire, sélectionne le capteur et envoie une demande de lecture du scratchpad */
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);
 
 /* Lecture du scratchpad */
  for (byte i = 0; i < 9; i++) {
    data[i] = ds.read();
  }
   
  /* Calcul de la température en degré Celsius */
  *temperature = ((data[1] << 8) | data[0]) * 0.0625; 
  
  // Pas d'erreur
  return READ_OK;
}

 float getPh(){
   static unsigned long samplingTime = millis();
   static unsigned long printTime = millis();
   static float pHValue,voltage;
   
   if(millis()-samplingTime > intervalMesure){
     tableauPh[indexPh++]=analogRead(SensorPin);
     if(indexPh==tailleEchantillon)
      indexPh=0;
     voltage = moyenne(tableauPh, tailleEchantillon)*5.0/1024;
     pHValue = 3.5*voltage+Offset;
     samplingTime=millis();
   }
  
   if(millis() - printTime > intervalConvertion)  //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
   {
   Serial.print("Voltage:");
   Serial.print(voltage,2);
   Serial.print("  pH value: ");
   Serial.println(pHValue,2);
   digitalWrite(LED,digitalRead(LED)^1);
   printTime=millis();
   }

   return pHValue;
 }
 double moyenne(int* arr, int number){
 int i;
 int max,min;
 double avg;
 long amount=0;
 if(number<=0){
  Serial.println("Error number for the array to avraging!/n");
  return 0;
}

 if(number<5){  //less than 5, calculated directly statistics
  for(i=0;i<number;i++){
    amount+=arr[i];
  }
  avg = amount/number;
  return avg;
 }else{
  
   if(arr[0]<arr[1]){
    min = arr[0];max=arr[1];
   }
   else{
     min=arr[1];max=arr[0];
    }
    
 for(i=2;i<number;i++){
  if(arr[i]<min){
    amount+=min;  //arr<min
    min=arr[i];
  }else {
    if(arr[i]>max){
      amount+=max;  //arr>max
      max=arr[i];
    }else{
      amount+=arr[i]; //min<=arr<=max
    }
  }//if

 }//for

 avg = (double)amount/(number-2);

 }//if

 return avg;

}


void setup() {
  // put your setup code here, to run once:
 pinMode(LED,OUTPUT);  
 Serial.begin(9600); 
}

void loop() {
   float temperature;
  /* Lit la température ambiante à ~1Hz */
  if (getTemperature(&temperature, true) != READ_OK) {
    Serial.println(F("Erreur de lecture du capteur"));
    return;
  }

  /* Affiche la température */
  Serial.print(F("Temperature : "));
  Serial.print(temperature, 2);
  Serial.write('*'); // Caractère degré
  Serial.write('C');
  Serial.println(", ph :  ");
  Serial.println(getPh());

  
}
