/*
Kakas-Raab Train 2022 09.8s
Autonomous Rail - modul sorter
Arduino nano
- HW: 
2x servo SG90
1x button
1x RGB senzor
*/
#include <Servo.h>

#define pinS0 4
#define pinS1 5
#define pinS2 6
#define pinS3 7
#define pinOut 8
#define pinButton 3

Servo servo1;
Servo servo2;

int poc = 0;     //jedna promenna na pocitani cycklu
int naloz = 11;  //naklad k vytridení
int nalozeno = 0;

void setup() {
  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(pinOut, INPUT);
  pinMode(pinButton, INPUT_PULLUP); // konfigurace start tlacitka
  digitalWrite(pinS0,HIGH);
  digitalWrite(pinS1,LOW);
  servo1.attach(9);
  servo1.write(115);
  servo2.attach(10);
  servo2.write(160);
  Serial.begin(9600);
}

void loop() {
  int frekvenceCervena, frekvenceZelena, frekvenceModra;
  int btnState = digitalRead(pinButton);
if (btnState == 1 && nalozeno == 1) {

    Serial.print("button state ");
    Serial.println(btnState);
    
    Serial.print("nalozeno ");
    Serial.println(nalozeno);
    delay (2000);
    nalozeno = 0;
}
btnState = digitalRead(pinButton);
if (btnState == 1 && nalozeno == 0) {
 for (poc = 0; poc<naloz; poc++){
    Serial.print("pocitadlo ");
    Serial.println(poc);
    
    Serial.print("button state nenalozeno ");
    Serial.println(btnState);
    
    Serial.print("nalozeno ");
    Serial.println(nalozeno);

  servo1.write(90);
  delay(1000);
  delay(1000);
  //for (int i = 110; i > 55; i--) {
   
    //delay(15);
  //}
  // měření červené barvy
  digitalWrite(pinS2,LOW);
  digitalWrite(pinS3,LOW);
  delay(50);
  frekvenceCervena = pulseIn(pinOut, LOW);
  // měření zelené barvy
  digitalWrite(pinS2,HIGH);
  digitalWrite(pinS3,HIGH);
  delay(50);
  frekvenceZelena = pulseIn(pinOut, LOW);
  // měření modré barvy
  digitalWrite(pinS2,LOW);
  digitalWrite(pinS3,HIGH);
  delay(50);
  frekvenceModra = pulseIn(pinOut, LOW);
  if (frekvenceCervena < 60) {
   
    servo2.write(45);
  }
  if (frekvenceModra < 60 && frekvenceCervena > 60) {
    servo2.write(100);
  }
  if (frekvenceCervena < 60 && frekvenceZelena < 60 && frekvenceModra < 60) {
    servo2.write(160);
  }
   servo1.write(115);
  delay(1000);
  servo1.write(52); 
  delay(1000);
  
  Serial.print("R: ");
  Serial.print(frekvenceCervena);
  Serial.print(" | G: ");
  Serial.print(frekvenceZelena);
  Serial.print(" | B: ");
  Serial.print(frekvenceModra);
  if (frekvenceCervena < 120) {
    Serial.print(" | Detekce cervene. ");
  }
  if (frekvenceZelena < 60) {
    Serial.print(" | Detekce zelene. ");
  }
  if (frekvenceModra < 60) {
    Serial.print(" | Detekce modre. ");
  }
  if (frekvenceCervena < 60 & frekvenceZelena < 60 ) {
    Serial.print(" | Detekce zlute. ");
  }
  Serial.println();
  delay(850);
  
 }
 nalozeno = nalozeno + 1;
}




}
