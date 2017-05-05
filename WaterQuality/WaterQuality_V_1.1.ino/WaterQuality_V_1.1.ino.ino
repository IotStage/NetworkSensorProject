
/*
 # Cet code rassemble tous les capteurs
 # Ver   : 1.1
 # Product: analog pH meter, turbidity, ORP, EC Meter, Temperature, anemometre, rain jauge
 # Author : Bassirou NGOM
 # email : bassiroungom26@gmail.com
 */

 /*************************
  *  inclusion
  * **********************/

#include <OneWire.h>

  /***********************
  *  Constantes          *
  * **********************/
#define SensorPinPH A0
#define SensorPinEC A1
#define SensorPinTurbidity A3
#define SensorPinORP A3
const byte BROCHE_ONEWIRE = 2;

/*************************
  *  constantes PH       *
  * **********************/
#define OffsetPH 0.00  //compensation de la deviation
#define LED 13      //le pin du led sur la carte
#define intervalMesure 20 //intervalle de ou on lit les valeurs sur le pin SensorPinPH
#define intervalConvertion 800 //intervalle ou les valeurs prises seront convertis en ph
#define tailleEchantillon  40  //taille des echantillons
int tableauPh[tailleEchantillon];  //tableau stokage des echantillons
int indexPh=0; 

/***************************************
  *  constantes EC et tamperature      *
  * ************************************/
#define StartConvert 0
#define ReadTemperature 1

const byte numReadings = 20;     //the number of sample times
//byte SensorPinEC = A1;  //EC Meter analog output,pin on analog 1
byte DS18B20_Pin = 7; //DS18B20 signal, pin on digital 7
unsigned int AnalogSampleInterval=25,printInterval=700,tempSampleInterval=850;  //analog sample interval;serial print interval;temperature sample interval
unsigned int readings[numReadings];      // the readings from the analog input
byte index = 0;                  // the index of the current reading
unsigned long AnalogValueTotal = 0;                  // the running total
unsigned int AnalogAverage = 0,averageVoltage=0;                // the average
unsigned long AnalogSampleTime,printTime,tempSampleTime;
float temperature,ECcurrent; 

OneWire dsEC(DS18B20_Pin);  // on digital pin 2
/* Code de retour de la fonction getTemperature() */
enum DS18B20_RCODES {
  READ_OK,  // Lecture ok
  NO_SENSOR_FOUND,  // Pas de capteur
  INVALID_ADDRESS,  // Adresse reçue invalide
  INVALID_SENSOR  // Capteur invalide (pas un DS18B20)
};

/* Création de l'objet OneWire pour manipuler le bus 1-Wire */
OneWire ds(BROCHE_ONEWIRE);


/***********************
  *  constantes ORP    *
  * ********************/
#define VOLTAGE 5.00    //system voltage
#define OFFSET 7        //zero drift voltage
//#define LED 13         //operating instructions

double orpValue;

#define ArrayLenth  40    //times of collection
#define orpPin 1          //orp meter output,connect to Arduino controller ADC pin

int orpArray[ArrayLenth];
int orpArrayIndex=0;


/******************************
  * declaration des Fonctions *
  * ***************************/
byte getTemperature(float *temperature, byte reset_search);
float getPh();
double moyenne(int* arr, int number);
float getTurbidity();
double avergearray(int* arr, int number);
int getORP();
void initEC();
float TempProcess(bool ch);
float getEC();

//

void setup(){
  
}

void loop()
{
  
}





  
