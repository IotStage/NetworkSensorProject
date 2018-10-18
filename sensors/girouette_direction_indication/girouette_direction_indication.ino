const int pinGirouette = A0;

void setup() {
Serial.begin(9600);
//pinMode(7, INPUT);
}

void loop() {

Serial.print(getGirouetteValue());
Serial.print(",");
Serial.println(getDirectionVent()/180.0);
delay(10);


}

float getGirouetteValue(){
  return analogRead(pinGirouette)*(5.0/1023.0);
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

