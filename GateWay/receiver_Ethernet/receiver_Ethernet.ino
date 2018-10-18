#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>


// parametres shield ethernet
//IPAddress ip(172,16,0,101);
IPAddress ip(10,2,1,11);
EthernetClient client;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server [] = "10.2.1.10";

int taille=2;
String data = "";


void setup()
{
  
  Serial.begin(9600);
  initialisation();
  Wire.begin(4);       // Rejoindre le bus à l'adresse #4
  Wire.onReceive(receiveEvent); // Preparer une fonction spécifique a la reception de donnee
  
}


void loop()
{
 
}


/**
 * Initialiser les modules
 */
void initialisation(){

  Serial.println("Intialisation...");

  //initialisation du module ethernet
  /*if(!Ethernet.begin(mac)){
    Ethernet.begin(mac, ip);
    Serial.print("parametrage avec ip fixe: ");
    Serial.println(ip);
  }else{
    Serial.println("parametrage avec dhcp");
  }*/
  Ethernet.begin(mac, ip);
  Serial.println("ethernet initialisé");
  
}



/**
 * Envoyer des données sur le réseau ethernet
 */
void sendEthernetData(String donnees){

  //formatter les donnees
  donnees.replace(" ", "%20");
  
  //se connecter
  client.connect(server, 80);
  Serial.println("Connexion...");

  //envoyer les donnees
  Serial.println("envoi des donnees:\n---------------------------------------");
  Serial.println(donnees);
  Serial.println("---------------------------------------------");
  
  client.print("GET /lampeadair/lampeadair.php?donnees=");
  client.print(donnees);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: keep-alive");
  client.println();

   //se deconnecter
  Serial.println("Deconnexion...");
  client.stop();
}

// Fonction qui s execute si quelque chose est present sur l interface
void receiveEvent(int taille){
  
  data = "";
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character
    data += c;
    //Serial.print(c);  
  }
  
  //Serial.println(" ");     // print the character
  //Serial.println(data);
  sendEthernetData(data);
  
}

