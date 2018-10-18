unsigned long tempsDebut=0;
unsigned long tempsFin=0;
long nbTour=0;
boolean etatPrecedent;
boolean etatActuel;


void setup() {
Serial.begin(9600);
pinMode(5, INPUT);
//pinMode(13, OUTPUT);
}

void loop() {
  getNBTour();

  //Serial.print("la vitesse du vent est : ");
  //
  
Serial.println(2*3.14*7.2*0.01*nbTour/4, 4);
  //Serial.println(" m/s");
  
  
}

void getNBTour(){
  nbTour=0;
  etatActuel = digitalRead(5);
  if(etatActuel==1 ){
    tempsDebut=millis();
    while(millis()-tempsDebut<1000){
      
      int etat = digitalRead(5);
      //digitalWrite(13, etat);
      nbTour = (etatActuel !=  etat) ? nbTour+1 : nbTour; 
      etatActuel = etat;   
      //Serial.print(etat);
      //Serial.print(" , ");
      //Serial.println(nbTour);
    }
    
  }
}

