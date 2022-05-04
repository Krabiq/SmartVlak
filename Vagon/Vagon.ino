/*
Kakas-Raab Train 2022 09.8w
Autonomous Rail - modul wagons
Arduino nano
- HW: 
2x servo SG90
2x button
*/
#include <Servo.h>

int button1 = 5;                  // konfigurace PIN - tlacitka na vagonu 1
int button2 = 6;                  // konfigurace PIN - tlacitka na vagonu 2
int btn1State = 0;
int btn2State = 0;
int vylozeno1 = 0;
int vylozeno2 = 0;
int pos = 0;

Servo servo1;
Servo servo2;

void setup() {
  Serial.begin(9600);             // konfigurace rychlosti serioveho portu console;
  Serial.println();
  Serial.println("Start vagony");

  pinMode(button1, INPUT_PULLUP); // konfigurace vagonu 1
  servo1.attach(10);
  servo1.write(0);

  pinMode(button2, INPUT_PULLUP); // konfigurace vagonu 2
  servo2.attach(11);
  servo2.write(0);
}

void loop() {
  btn1State = digitalRead(button1);
  btn2State = digitalRead(button2);
  /*
  Serial.print(btn1State);
  Serial.println(btn2State);
  */

  if (btn1State == 1 && vylozeno1 == 0) {
    delay(1000);
    vyklopka1(90);
    delay(2000);
    Serial.println("vracim");
    vyklopka1(-90);
    vylozeno1 = vylozeno1 + 1;
  }
  if (btn1State == 0 && vylozeno1 == 1) {
     vylozeno1 = vylozeno1 - 1;
  }

  if (btn2State == 1 && vylozeno2 == 0) {
    delay(1000);
    vyklopka2(-90);
    delay(2000);
    Serial.println("vracim");
    vyklopka2(90);
    vylozeno2 = vylozeno2 + 1;
  }
  if (btn2State == 0 && vylozeno2 == 1) {
     vylozeno2 = vylozeno2 - 1;
  }
}

void vyklopka1(int uhel) {
  if (uhel > 0) {
  Serial.print(" vyklopka <= o uhel "); 
  Serial.print(uhel);
  Serial.println();
  for(pos = 0; pos <= uhel; pos += 1) //je od úhlu 0 do úhlu 180 (360)
  {
    servo1.write(pos);     //natočení motoru na aktuální úhel
    delay(20);             //chvilka čekání než se motor natočí
  }
  } else {
  uhel = abs(uhel);
  Serial.print(" vyklopka => o uhel "); 
  Serial.print(uhel);
  Serial.println(); 
  for(pos = uhel; pos >= 0; pos -= 1) //je od úhlu 180 (360) zpět do úhlu 0
  {
    servo1.write(pos);     //natočení motoru na aktuální úhel
    delay(20);             //chvilka čekání než se motor natočí
  }
 }
}

void vyklopka2(int uhel) {
  if (uhel > 0) {
  Serial.print(" vyklopka 2 <= o uhel "); 
  Serial.print(uhel);
  Serial.println();
  for(pos = 0; pos <= uhel; pos += 1) //je od úhlu 0 do úhlu 180 (360)
  {
    servo2.write(pos);     //natočení motoru na aktuální úhel
    delay(20);             //chvilka čekání než se motor natočí
  }
  } else {
  uhel = abs(uhel);
  Serial.print(" vyklopka 2 => o uhel "); 
  Serial.print(uhel);
  Serial.println(); 
  for(pos = uhel; pos >= 0; pos -= 1) //je od úhlu 180 (360) zpět do úhlu 0
  {
    servo2.write(pos);     //natočení motoru na aktuální úhel
    delay(20);             //chvilka čekání než se motor natočí
  }
 }
}
