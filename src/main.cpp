#include <Arduino.h>
#include <DallasTemperature.h>
#include <OLED_I2C.h>
#include <DS1307.h>
#include <EEPROM.h>

extern uint8_t UkrFont[];
extern uint8_t MegaNumbers[];
extern uint8_t SmallFont[];

long frametime1 = 0;
int interval = 500;
int intervalblink = 500;
long blinktime = 0;
long dalastime = 0;
long zaslonka = 0;
long zaslonkaleto = 0;
long intervaldalas = 10000;
bool frametimeon = false;
bool blinks= LOW;
bool winter = false;
bool klocker = false;
bool leto = false;
int fan = 11;
int cel;
int cel2;
int aktuatorUP = 2;
int aktuatorDOWN = 3;

byte zadtemp = EEPROM.read(1);
byte menu = EEPROM.read(2);



DS1307 rtc(A0, A1);
Time t;
OLED  myOLED(SDA, SCL, 8);
OneWire oneWire(10); // вход датчиков 18b20


DallasTemperature ds(&oneWire);
DeviceAddress sensor1 = {0x28, 0xFF, 0xA2, 0x81, 0x87, 0x16, 0x3, 0x12};
DeviceAddress sensor2 = {0x28, 0xFF, 0x2F, 0xF3, 0x87, 0x16, 0x3, 0x9A};



void setup() {

  myOLED.begin();
  rtc.halt(false);
  Serial.begin(9600);
  ds.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(aktuatorUP, OUTPUT);
  pinMode(aktuatorDOWN, OUTPUT);
//------------------------------------------------------------------------------
  digitalWrite(aktuatorDOWN,HIGH);
  digitalWrite(aktuatorUP,HIGH);
}

void dalas(){
    cel = ds.getTempC(sensor1);
    cel2 = ds.getTempC(sensor2);

    ds.requestTemperaturesByAddress(sensor1);
    ds.requestTemperaturesByAddress(sensor2);
}
void grad(){

  myOLED.clrScr();
  myOLED.setFont(UkrFont);
  myOLED.print("Ctycjh 1",  4,  0);
  myOLED.print("Ctycjh 2",  75, 0);
  myOLED.setFont(MegaNumbers);
  myOLED.print(String(cel), 4, 12);
  myOLED.print(String(cel2),75,12 );
  myOLED.setFont(UkrFont);
  myOLED.print("NTVGTHFNEHF CTYCJHSD", CENTER, 57);
  myOLED.update();
}

void timer(long t, int intervals, bool x) {
if (millis() - t > intervals){
  t = millis();
      if(x == false){
        x  = true;
      }
      else{
        x = false;
      }
}

}


void blink() {
  if(millis() - blinktime > intervalblink){
    blinktime = millis();

  if(blinks == LOW){
  blinks = HIGH;
}
  else{
  blinks = LOW;
}
}
digitalWrite(LED_BUILTIN, blinks);
}



void calendar() {

if(millis()-frametime1 > interval) {
    frametime1 = millis();
      if(frametimeon == false){
      frametimeon = true;
      myOLED.clrScr();
      myOLED.setFont(UkrFont);
      t = rtc.getTime();
      switch (t.dow)
      {
        case 1:     myOLED.print("GJYTLSKJR", CENTER, 0);    break;
        case 2:     myOLED.print("DSDNJHJR", CENTER, 0);        break;
        case 3:     myOLED.print("CTHTLF", CENTER, 0);          break;
        case 4:     myOLED.print("XTNDTH", CENTER, 0);        break;
        case 5:     myOLED.print("GZNYBWZ", CENTER, 0);        break;
        case 6:     myOLED.print("CE<JNF", CENTER, 0);        break;
        case 7:     myOLED.print("YTLSKZ", CENTER, 0);    break;
      }
      String stringOne = rtc.getTimeStr();
      myOLED.setFont(MegaNumbers);
      myOLED.print(stringOne.substring(0, 2), 4, 12);
      myOLED.print("/", 51, 12);
      myOLED.print(stringOne.substring(3, 5), 75, 12);
      myOLED.setFont(UkrFont);
      switch (t.mon)
      {
        case 1:
          myOLED.print(String(t.date), 30, 57);
          myOLED.print("CSXYZ", CENTER, 57);
          myOLED.print(String(t.year), 88, 57);
          break;
        case 2:
          myOLED.print(String(t.date), 26, 57);
          myOLED.print("K>NJUJ", CENTER, 57);
          myOLED.print(String(t.year), 92, 57);
          break;
        case 3:
          myOLED.print(String(t.date), 30, 57);
          myOLED.print("<THTPYZ", CENTER, 57);
          myOLED.print(String(t.year), 88, 57);
          break;
        case 4:
          myOLED.print(String(t.date), 30, 57);
          myOLED.print("RDSNYZ", CENTER, 57);
          myOLED.print(String(t.year), 88, 57);
          break;
        case 5:
          myOLED.print(String(t.date), 36, 57);
          myOLED.print("NHFDYZ", CENTER, 57);
          myOLED.print(String(t.year), 82, 57);
          break;
        case 6:
          myOLED.print(String(t.date), 35, 57);
          myOLED.print("XTHDYZ", CENTER, 57);
          myOLED.print(String(t.year), 81, 57);
          break;
        case 7:
          myOLED.print(String(t.date), 35, 57);
          myOLED.print("KBGYZ", CENTER, 57);
          myOLED.print(String(t.year), 81, 57);
          break;
        case 8:
          myOLED.print(String(t.date), 28, 57);
          myOLED.print("CTHGYZ", CENTER, 57);
          myOLED.print(String(t.year), 90, 57);
          break;
        case 9:
          myOLED.print(String(t.date), 24, 57);
          myOLED.print("DTHTCYZ", CENTER, 57);
          myOLED.print(String(t.year), 94, 57);
          break;
        case 10:
          myOLED.print(String(t.date), 26, 57);
          myOLED.print(":JDNYZ", CENTER, 57);
          myOLED.print(String(t.year), 92, 57);
          break;
        case 11:
          myOLED.print(String(t.date), 28, 57);
          myOLED.print("KBCNJGFLF", CENTER, 57);
          myOLED.print(String(t.year), 90, 57);
          break;
        case 12:
          myOLED.print(String(t.date), 26, 57);
          myOLED.print("UHELYZ", CENTER, 57);
          myOLED.print(String(t.year), 92, 57);
          break;
      }
      myOLED.update();
    }
else{
      frametimeon = false;
      myOLED.setFont(MegaNumbers);
      myOLED.print("-", 51, 12);
      myOLED.update();
    }
}
}



void dg(){
  int x_position;
  int y_position;

  const int X_THRESHOLD_LOW = 450;
  const int X_THRESHOLD_HIGH = 550;
  const int Y_THRESHOLD_LOW = 450;
  const int Y_THRESHOLD_HIGH = 550;

  int x_direction;
  int y_direction;
  int menupos = 0;


  x_direction = 0;
  y_direction = 0;

  x_position = analogRead(A2);
  y_position = analogRead(A3);

//-----------------------------------------------------------------------------

if (x_position > X_THRESHOLD_HIGH){
                                  x_direction = 1;
  }

else if (x_position < X_THRESHOLD_LOW){
                                  x_direction = -1;
  }

if (y_position > Y_THRESHOLD_HIGH){
                                  y_direction = 1;
  }

else if (y_position < Y_THRESHOLD_LOW) {
                                  y_direction = -1;
  }
//------------------------------------------------------------------------------
  if (x_direction == -1) {
    if (y_direction == -1) {
                            Serial.println("left-down");
                            }
                            else if (y_direction == 0) {
                                                        Serial.println("left");
                                                        menupos++;
                                                        }
                                                         else {
                                                                // y_direction == 1
                                                                Serial.println("left-up");
                                                              }
  }

  else if (x_direction == 0) {
    if (y_direction == -1) {
                            if(menu == 2){
                              zadtemp--;
                              EEPROM.write(1,zadtemp);
                            }
                            Serial.println("down");
                            }
                            else if (y_direction == 0) {

                                                        Serial.println("centered");
                                                        }
                                                        else {
                                                              if(menu == 2){
                                                                zadtemp++;
                                                                EEPROM.write(1,zadtemp);
                                                              }// y_direction == 1
                                                              Serial.println("up");
                                                              }
}

  else {
    // x_direction == 1
    if (y_direction == -1) {
                            Serial.println("right-down");
                            }
                            else if (y_direction == 0) {
                                                        Serial.println("right");
                                                        menupos--;
                                                      }
                                                      else {
                                                            // y_direction == 1
                                                            Serial.println("right-up");

                                                            }
     }
//------------------------------------------------------------------------------
     if (menupos == 1){
                       menu = menu + 1;
                     }
     if (menupos == -1){
                       menu = menu - 1;
                       }
     if (menu > 2){
       menu  = 0;
     }
     if (menu < 0){
       menu = 0;
     }
}
//------------------------------------------------------------------------------


void  fann(){

int  fanstep = 20;
int raznicatemp = zadtemp - cel2;
byte fanspeed = 255;

if (millis() > 3000 && millis() < 4000 && cel <= 22){

  digitalWrite(aktuatorUP,LOW);
  winter = true;
}
if (millis() > 4000 && millis() < 5000 && cel <= 22){
  digitalWrite(aktuatorUP,HIGH);
}

if (millis() > 3000 && millis() < 4000 && cel >= 23){

  digitalWrite(aktuatorDOWN,LOW);
  leto = true;
}
if (millis() > 4000 && millis() < 5000 && cel >= 23){
  digitalWrite(aktuatorDOWN,HIGH);
}





//  digitalWrite(fan,255);// виключений
//digitalWrite(fan,0);// включений

switch (raznicatemp) {
  case -1:
    fanspeed = fanspeed - fanstep*3;
    break;
  case -2:
    fanspeed = fanspeed - fanstep*4;
    break;
  case -3:
    fanspeed = fanspeed - fanstep*5;
    break;
  case -4:
    fanspeed = fanspeed - fanstep*6;
    break;
  case -5:
    fanspeed = fanspeed - fanstep*7;
    break;
  case -6:
    fanspeed = fanspeed - fanstep*8;
    break;
  case -7:
    fanspeed = fanspeed - fanstep*9;
    break;
  case -8:
    fanspeed = fanspeed - fanstep*10;
    break;
  case -9:
    fanspeed = fanspeed - fanstep*11;
   break;
  case -10:
    fanspeed = 0;
   break;
  case 1:
    fanspeed = 255;
   break;
  case 2:
      fanspeed = fanspeed - fanstep*3;
   break;
  case 3:
    fanspeed = fanspeed - fanstep*4;
   break;
  case 4:
    fanspeed = fanspeed - fanstep*5;
   break;
  case 5:
    fanspeed = fanspeed - fanstep*6;
   break;
  case 6:
    fanspeed = fanspeed - fanstep*7;
    break;
  case 7:
    fanspeed = fanspeed - fanstep*8;
    break;
  case 8:
    fanspeed = fanspeed - fanstep*9;
    break;
  case 9:
    fanspeed = fanspeed - fanstep*10;
    break;
  case 10:
    fanspeed = 0;
    break;
  default:
      fanspeed = 255;
  }

analogWrite(fan,fanspeed);
}

void salon() {
  myOLED.clrScr();
  myOLED.setFont(UkrFont);
  myOLED.print("PFLFYF NTVGTHFNEHF",  CENTER,  0);
  myOLED.setFont(MegaNumbers);
  myOLED.print(String(zadtemp),  CENTER,  12);
  if(winter == true){
    myOLED.setFont(UkrFont);
    myOLED.print("Ghjuhsd Fdnj",  CENTER,  57);
  }
  if (winter == false) {
    myOLED.setFont(UkrFont);
    myOLED.print("Ht;bv Ksnj",  CENTER,  57);
  }
  myOLED.update();
}


void m(){
  if (menu == 0){
  calendar();
  EEPROM.write(2, menu);
  }
 if (menu == 1){
  grad();
  EEPROM.write(2, menu);
 }
 if (menu == 2){
  salon();
  EEPROM.write(2, menu);
 }
}



void loop() {

  blink();
  dg();
  m();
  fann();
  dalas();
}