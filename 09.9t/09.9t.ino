/*
Kakas-Raab Train 2022 09.9t
Autonomous Rail - modul train
Autonomous Rail - modul connect

Arduino mega2560

   [TRAIN v8.2]
   PIN table - HW pripojeni
   Tlacitka PIN 2,3,4,5 10k - GND
   Output RELAY 9,10,11,12  - 4 RELAY BOARD
   LED onBoard - reference 13
   Servo SG90  - 7
  
   [CONNECT v0.2]
   PIN table - HW pripojeni
   ESP8266 - GND, RX1,TX1, Vin+
*/

// konstanty - definice PINu
const int buttonPinA = 2;     // pin tlacitka stanice A
const int buttonPinB = 3;     // pin tlacitka stanice B
const int buttonPinC = 4;     // pin tlacitka stanice C
const int buttonPinD = 5;     // pin tlacitka stanice D
const int ledPin =  13;       // LED pin
const int BeepPin = 53;       // sirenka 
const int relayPinA = 12;     // Relay In1
const int relayPinB = 11;     // Relay In2
const int relayPinC = 10;     // Relay In3 rezerva
const int relayPinD = 9;      // Relay In4 rezerva

const int cervena = 35;        //pin 35 je semafor červená
const int zluta = 33;          //pin 33 je semafor žlutá
const int zelena = 31;         //pin 31 je semafor zelená

// promenne:
int buttonStateA = 0;         // variable for reading the pushbutton A status
int buttonStateB = 0;         // variable for reading the pushbutton B status
int buttonStateC = 0;         // variable for reading the pushbutton C status
int buttonStateD = 0;         // variable for reading the pushbutton D status

int poc;                      // promenna na pocitani cycklu
int rychlost = 0;             // aktualni rychlost
int nalozeno = 0;       

bool StavimevA = false;        // nestavime ve stanici A
bool StavimevB = true;         // stavime ve stanici B
bool StavimevC = false;        // nestavime ve stanici C
bool StavimevD = false;        // nestavime ve stanici D


// implementace serva SG90
#include <Servo.h>            //zahrnutí knihovny pro ovládání servo motoru
Servo vyh1servo;              //každý motor má svou instanci třídy Servo
int pos = 0;                  //proměnná obsahující pozici motoru (úhel natočení)

// implementace WiFi ESP8266
#include <ThingSpeak.h>       // knihovna ThingSpeak for IoT Projects MATLAB
#define DEBUG false


String mySSID = "robotika";              // WiFi SSID skola
String myPWD = "nemuzuprozradit";        // WiFi Password


String myAPI = "LP641RFUZW6CAV1B";       // API Key
String myHOST = "api.thingspeak.com";
String myHOST2 = "www.seznam.cz";        // testovaci HOST2
String myPORT = "80";
String myFIELD1 = "field1";
String myFIELD2 = "field2";
String myFIELD3 = "field3";
String myFIELD4 = "field4";
String myFIELD5 = "field5";
String myFIELD6 = "field6";
String myFIELD7 = "field7";
String myFIELD8 = "field8";

int sendVal1 = 0;
int sendVal2 = 0;
int sendVal3 = 0;
int sendVal4 = 0;
int sendVal5 = 0;
int sendVal6 = 0;
int sendVal7 = 0;
int sendVal8 = 0;

void setup() {

  Serial.begin(9600);             // konfigurace rychlosti serioveho portu console;
  Serial.println();
  
  Serial.println(".___<>_________________");
  Serial.println(" /---||---||---||---||-\\\\");
  Serial.println("|   Kakas-Raab TRAIN 2022  |");
  Serial.println(" \\_--O=O----------O=O--_/");
  Serial.println("################################ universal ###############");
  

  // inicializace LED jako vystupu:
  pinMode(ledPin, OUTPUT);
  // inicializace spinacu zastavek jako vstupu::
  pinMode(buttonPinA, INPUT);
  pinMode(buttonPinB, INPUT);
  pinMode(buttonPinC, INPUT);
  pinMode(buttonPinD, INPUT);
  // inicializace RELAY jako vystupu:
  pinMode(relayPinA, OUTPUT); 
  pinMode(relayPinB, OUTPUT);
  pinMode(relayPinC, OUTPUT);  // rezerva
  pinMode(relayPinD, OUTPUT);  // rezerva
  // inicializace Sirenky jako vystupu
  pinMode(BeepPin, OUTPUT);
  // Semafor vystupy
  pinMode(cervena, OUTPUT);  //červená je výstup
  pinMode(zluta, OUTPUT);  //žlutá je výstup
  pinMode(zelena, OUTPUT);  //zelená je výstup
  digitalWrite(zelena, HIGH);  //zapnout zelenou
  // servo vyh1 na PIN 7
  vyh1servo.attach(7);   //tento motor je připojen na pin 7
  // servo vyh1 iniciace do pozice 0
  Serial.println("Inicializace vyhybek");
  vyh1servo.write(pos);
  // Incializace WiFi spojeni
  Serial.println("Inicializace WiFi ");
  Serial1.begin(115200);
  //espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                   //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  delay(1000);
  espData("AT+CIFSR", 1000, DEBUG);
  Serial.println("Inicializace WiFi kompletni ");
  Serial.println();

  // START jizdy
  Serial.println("Start jizdy");
  Serial.println();
  jed(2);
  pipni (3,100);
  pipni (1,500);

}
 
// ============================================================================================
// HLAVNI SMYCKA
// ============================================================================================
void loop() {
  // precte stav pushbutton ABCD value:
  buttonStateA = digitalRead(buttonPinA);
  buttonStateB = digitalRead(buttonPinB);
  buttonStateC = digitalRead(buttonPinC);
  buttonStateD = digitalRead(buttonPinD);

  // stanice A (If it is, the buttonState A is HIGH:)
  if (buttonStateA == HIGH && StavimevA) {
    stuj();
    stavIny();                         // zapise stav ABCD do serial console
    Serial.println("A vstup zmena ");
    // scenar stanice A
    //
    pipni (1,50);
    sendVal5 = 1;
    ThingSpeakReport ();
    Serial.println("1/2 vykladka ve stanici A");
    delay (5000);
    nalozeno = nalozeno -1;
    sendVal2 = nalozeno;
    vyhybka1(177);
    semafor(1);
    StavimevB = false;
    jed(2);
  
     //
     do {                              // pokud trva sepnuti Inu A
      blikani(1);
      delay(1000);
      Serial.println("A trva,");
      buttonStateA = digitalRead(buttonPinA);
     } while (buttonStateA == HIGH);
    // stav A obnoven 
       Serial.println("A vstup konec - opusteni stanice ");
       sendVal5 = 0;
       delay(5000);
      Serial.println("A - C konec zatlumení");
         
  // stanice B (If it is, the buttonState B is HIGH:)
  } else if (buttonStateB == HIGH && StavimevB) {     
    stuj();
    stavIny();                                 // zapise stav ABCD do serial console
    Serial.println("B vstup zmena ");
    // scenar stanice B
    //
    pipni (2,50);
    sendVal6 = 1;
    ThingSpeakReport ();
    Serial.println("nakladka ve stanici B");
    delay (60000);
    nalozeno = nalozeno +2;
    sendVal2 = nalozeno;
    StavimevA = true;
    StavimevB = false;
    StavimevC = true;
    jed(2);

     do {                              // pokud trva sepnuti Inu B
      blikani(2);
      delay(1000);
      Serial.println("B trva,");
      buttonStateB = digitalRead(buttonPinB);
     } while (buttonStateB == HIGH);
    // stav B obnoven 
     Serial.println("B vstup konec - opusteni stanice ");
     sendVal6 = 0;
  
    // stanice C (If it is, the buttonState C is HIGH:)
  } else if (buttonStateC == HIGH && StavimevC) {
    stuj();
    stavIny();                         // zapise stav ABCD do serial console
    Serial.println("C vstup zmena ");
    // scenar stanice C
    //
    pipni (3,50);
    
    sendVal7 = 1;
    ThingSpeakReport ();
    semafor (0);
    Serial.println("1/2 vykladka ve stanici C");
    delay (5000);
    nalozeno = nalozeno -1;
    sendVal2 = nalozeno;   
    StavimevA = false;
    StavimevB = true;
    vyhybka1(-177);
    jed(2);
         // pokud trva sepnuti Inu C
     do {
      blikani(3);
      delay(900);
      Serial.println("C trva,");
      buttonStateC = digitalRead(buttonPinC);
     } while (buttonStateC == HIGH);
     // stav C obnoven
     Serial.println("C vstup konec ");
     sendVal7 = 0;
  
  // stanice D (If it is, the buttonState D is HIGH:)    
  } else if (buttonStateD == HIGH && StavimevD) {
    stavIny();                         // zapise stav ABCD do serial console
    Serial.println("D vstup zmena ");
    // scenar stanice D
    //
    pipni (4,50);
    stuj();
    sendVal8 = 1;
    ThingSpeakReport ();
    delay(2000);
    Serial.println("cigarko ve stanici D");stuj();
    delay(2000);
jed(2);
    StavimevD = false;

    // pokud trva sepnuti Inu D
     do {
      blikani(4);
      delay(800);
      Serial.println("D trva,");
      buttonStateD = digitalRead(buttonPinD);
     } while (buttonStateD == HIGH);
     // stav D obnoven 
     Serial.println("D vstup konec ");
     sendVal8 = 0;
    
  } else {     
    // turn LED off - zadny aktivni vstup
    digitalWrite(ledPin, LOW);
  }
 // klidový stav provozu - LED OFF, kazdy 10000 cyklus vypise rychlost a posle ThingSpeakReport
   if (poc < 2000) {
    poc = poc +1;
  } else { 
    Serial.print("rychlost ");
    Serial.print(rychlost);
    Serial.print(" , ");
    Serial.print(StavimevA, BIN);
    Serial.print(StavimevB, BIN);
    Serial.print(StavimevC, BIN);
    Serial.print(StavimevD, BIN);
    Serial.println(" - ABCD stavime ve stanici"); 
    /*      ThingSpeak hodnoty reportu    */
    
           //sendVal = random(100); // Send a random number between 1 and 10
    sendVal3 = random(10);
    sendVal4 = random(10);

    ThingSpeakReport ();

    poc = 0;
  }

 delay(20); // zpomalení cele smycky | reakce vstupu
}

// ============================================================================================
// Methody
// ============================================================================================

void blikani(int a){        // blikani status LED
	// Serial.print(a);
  // Serial.println(" bliknuti LED,");
  for(poc=0;poc< a ;poc++){  //rychle blikani LED
      delay(20);             // X zhashutí, kdyz stav trvá
      digitalWrite(ledPin, LOW);
      delay(120);
      digitalWrite(ledPin, HIGH);
      }
}

void jed(int s){            // razeni rychlosti s omezovacem na 10
	Serial.print("aktualni rychlost ");
  Serial.println(rychlost);
	Serial.print("pozadovana rychlost +");
  Serial.print(s);
  if (rychlost+s > 10) {
    Serial.print(" je vyssi nez max. povolena rychlost");
     s = 10 - rychlost;
  }
  Serial.print(", radim");
  for(poc=0;poc< s ;poc++){  //pocet impulsu - rychlost
      digitalWrite(relayPinA, HIGH);
      delay(300);
      digitalWrite(relayPinA, LOW);
      delay(700);
      Serial.print(" "); 
      Serial.print(poc+1);       
      }
  Serial.println();
  rychlost=rychlost+s;
  sendVal1 = rychlost;
}

void stuj(){
	Serial.println("stuj ");
  digitalWrite(relayPinB, HIGH);
  delay(300);
  digitalWrite(relayPinB, LOW);
  delay(700);
  rychlost=0;
  sendVal1 = rychlost;         
}

void stavIny(){
     digitalWrite(ledPin, HIGH);      // turn LED on - nejaky vstup sepnut
     Serial.println();
     Serial.print(buttonStateA);    // A
     Serial.print(buttonStateB);    // B
     Serial.print(buttonStateC);    // C
     Serial.print(buttonStateD);    // D
     Serial.println(" - ABCD vlak ve stanici"); 
     Serial.print(StavimevA, BIN);
     Serial.print(StavimevB, BIN);
     Serial.print(StavimevC, BIN);
     Serial.print(StavimevD, BIN);
     Serial.println(" - ABCD stavime ve stanici"); 
}

void vyhybka1(int uhel) {
  if (uhel > 0) {
  Serial.print(" vyhybka => o uhel "); 
  Serial.print(uhel);
  Serial.println();
  for(pos = 0; pos <= uhel; pos += 1) //je od úhlu 0 do úhlu 180 (360)
  {
    vyh1servo.write(pos);  //natočení motoru na aktuální úhel
    delay(20);             //chvilka čekání než se motor natočí
  }
  } else {
  uhel = abs(uhel);
  Serial.print(" vyhybka <= o uhel "); 
  Serial.print(uhel);
  Serial.println(); 
  for(pos = uhel; pos >= 0; pos -= 1) //je od úhlu 180 (360) zpět do úhlu 0
  {
    vyh1servo.write(pos);  //natočení motoru na aktuální úhel
    delay(20);             //chvilka čekání než se motor natočí
  }
 }
}

void ThingSpeakReport(){
    Serial.println(">>> - Provadim ThingSpeakReport");
    //http://api.thingspeak.com/update?api_key=XVKHSUERMX3UPQD9&field1=73
    //String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD1 +"="+String(sendVal);
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD1 +"="+String(sendVal1) +"&"+ myFIELD2 +"="+String(sendVal2) +"&"+ myFIELD3 +"="+String(sendVal3) +"&"+ myFIELD4 +"="+String(sendVal4) +"&"+ myFIELD5 +"="+String(sendVal5) +"&"+ myFIELD6 +"="+String(sendVal6) +"&"+ myFIELD7 +"="+String(sendVal7) +"&"+ myFIELD8 +"="+String(sendVal8);
    espData("AT+CIPMUX=1", 1000, DEBUG);       //Allow multiple connections
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+2),1000,DEBUG);  
    Serial1.find(">"); 
    Serial1.println(sendData);
    Serial.print("Hodnota k odeslani: ");
    Serial.println(sendData);
    // Serial.print("Velikost k odeslani: ");
    // Serial.println(sendData.length());
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    Serial.println("<<< - ThingSpeakReport ukoncen");

}

String espData(String command, const int timeout, boolean debug)   // WiFi AT prikazy
{
   //Serial.print("AT Command ==> ");    // WiFi AT prikazy vypis do Serial Monitoru
   //Serial.print(command);
   //Serial.println("     ");
  
  String response = "";
  Serial1.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      response += c;
    }
  }

    // Serial.print(response);        // odpovedi z ESP8266, zakomentovano z duvodu prehlednosti

  return response;
}

void pipni (int pp, int pd){
  for(poc=0;poc< pp ;poc++){  //rychle pipnuti
    digitalWrite(BeepPin, HIGH);
    delay(pd);             
    digitalWrite(BeepPin, LOW);
    delay(pd);
    }
}

void semafor (int sem){
   if (sem == 1) {              //pokud je semafor doprava
    Serial.println("semafor -ooo ");
    digitalWrite(zelena, LOW);      //vypnout zelenou
    digitalWrite(zluta, HIGH);      //zapnout žlutou
    delay(500);                    //počkat 0.5 sekund
    digitalWrite(zluta, LOW);       //vypnout žlutou
    digitalWrite(cervena, HIGH);    //zapnout červenou
    } else {
    Serial.println("semafor ooo- ");
    digitalWrite(cervena, LOW);     //vypnout červenou
    digitalWrite(zluta, HIGH);      //zapnout žlutou
    delay(500);                    //počkat 0.5 sekund
    digitalWrite(zluta, LOW);       //vypnout žlutou
    digitalWrite(zelena, HIGH);     //zapnout zelenou
    delay(500);                    //počkat 0.5 sekund
  }
}
