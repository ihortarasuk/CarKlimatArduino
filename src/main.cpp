#include <Arduino.h>
#include <DallasTemperature.h>
#include <OLED_I2C.h>
#include <DS1307.h>
#include <EEPROM.h>
#include <Stepper.h>
#include <PID_v1.h>
bool minutes = false;
bool houres = false;
extern uint8_t UkrFont[];
extern uint8_t MegaNumbers[];
extern uint8_t SmallFont[];
double Setpoint2, Input2, Output2;
unsigned long frametime1 = 0;
unsigned int interval = 500;
unsigned int intervalblink = 500;
unsigned long blinktime = 0;
bool stepvork = false;
bool zaslonkastate = false;
bool frametimeon = false;
bool blinks= LOW;
bool winter = false;
bool leto = false;
bool pogoda = false;
bool nastroika = false;
const int steppoz = 2800;
const int pomp = 10;
unsigned short int fan = 11;
unsigned short int cel;
unsigned short int cel2;
short int hourse;
short int minute;
short int znach = 1;
short int stepvalue;
short int stepperval;
short int pohibka;
short int peremenS;
short int stepmotor;
short int stepvalueold;
const int buttonPin = 6;
int buttonPinstate = 0;
const int koncevik = 7;
int bootnstate = 0;
int koncpos = 0;
short int stepA;
short int stepB;
short int stepC;
short int speedfan;
byte zadtemp = EEPROM.read(1);
byte menu = EEPROM.read(2);

DS1307 rtc(A0, A1);
Time t;
OLED  myOLED(SDA, SCL, 8);
OneWire oneWire(9); // вход датчиков 18b20
Stepper myStepper(2048,5,3,2,4);
double Setpoint, Input, Output;
DallasTemperature ds(&oneWire);
DeviceAddress sensor2 = {0x28, 0xFF, 0xA2, 0x81, 0x87, 0x16, 0x3, 0x12};
DeviceAddress sensor1 = {0x28, 0xFF, 0x2F, 0xF3, 0x87, 0x16, 0x3, 0x9A};
PID myPID(&Input, &Output, &Setpoint,30,100,10,DIRECT);//создаем ПИД-регулятор
PID myPID2(&Input2, &Output2, &Setpoint2,2,5,1, REVERSE);



void setup() {
  myPID.SetOutputLimits(0,2800);
  myStepper.setSpeed(10);
  myOLED.begin();
  rtc.halt(false);
    //rtc.setDOW(THURSDAY);
    //rtc.setTime(18, 19, 0);
    //rtc.setDate(12,9, 2017);
  Serial.begin(9600);
  ds.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(pomp, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(koncevik,INPUT);
  myPID.SetMode(AUTOMATIC);
  myPID2.SetMode(AUTOMATIC);
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
  myOLED.print(String("GSXRF"),  4,  0);
  myOLED.print(String("CFKJY"),  75, 0);
  myOLED.setFont(MegaNumbers);
  myOLED.print(String(cel2), 4, 13);
  myOLED.print(String(cel),75,13 );
  myOLED.setFont(UkrFont);
  myOLED.print(String("NTVGTHFNEHF CTYCJHSD"), CENTER, 57);
  myOLED.update();
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

if (millis()-frametime1 > interval) {
    frametime1 = millis();
      if(frametimeon == false){
      frametimeon = true;
      myOLED.clrScr();
      myOLED.setFont(UkrFont);
      t = rtc.getTime();
      if (nastroika == true && houres == true && minutes == false){
          myOLED.print(String("YFKFINEDFYYZ UJLBY"), CENTER, 0);
      }
      if (nastroika == true && houres == false && minutes == true){
          myOLED.print(String("YFKFINEDFYYZ {DBKBY"), CENTER, 0);
      }
      if (nastroika == false && houres == false && minutes == false){
      switch (t.dow)
      {
        case 1:     myOLED.print(String("GJYTLSKJR"), CENTER, 0);       break;
        case 2:     myOLED.print(String("DSDNJHJR"), CENTER, 0);        break;
        case 3:     myOLED.print(String("CTHTLF"), CENTER, 0);          break;
        case 4:     myOLED.print(String("XTNDTH"), CENTER, 0);          break;
        case 5:     myOLED.print(String("GZNYBWZ"), CENTER, 0);         break;
        case 6:     myOLED.print(String("CE<JNF"), CENTER, 0);          break;
        case 7:     myOLED.print(String("YTLSKZ"), CENTER, 0);          break;
      }
    }
      String stringOne = rtc.getTimeStr();
      myOLED.setFont(MegaNumbers);
      myOLED.print(stringOne.substring(0, 2), 4, 13);
      myOLED.print(String("/"), 51, 12);
      myOLED.print(stringOne.substring(3, 5), 75, 13);
      myOLED.setFont(UkrFont);
      switch (t.mon)
      {
        case 1:
          myOLED.print(String(t.date), 30, 57);
          myOLED.print(String("CSXYZ"), CENTER, 57);
          myOLED.print(String(t.year), 88, 57);
          break;
        case 2:
          myOLED.print(String(t.date), 26, 57);
          myOLED.print(String("K>NJUJ"), CENTER, 57);
          myOLED.print(String(t.year), 92, 57);
          break;
        case 3:
          myOLED.print(String(t.date), 30, 57);
          myOLED.print(String("<THTPYZ"), CENTER, 57);
          myOLED.print(String(t.year), 88, 57);
          break;
        case 4:
          myOLED.print(String(t.date), 30, 57);
          myOLED.print(String("RDSNYZ"), CENTER, 57);
          myOLED.print(String(t.year), 88, 57);
          break;
        case 5:
          myOLED.print(String(t.date), 36, 57);
          myOLED.print(String("NHFDYZ"), CENTER, 57);
          myOLED.print(String(t.year), 82, 57);
          break;
        case 6:
          myOLED.print(String(t.date), 35, 57);
          myOLED.print(String("XTHDYZ"), CENTER, 57);
          myOLED.print(String(t.year), 81, 57);
          break;
        case 7:
          myOLED.print(String(t.date), 35, 57);
          myOLED.print(String("KBGYZ"), CENTER, 57);
          myOLED.print(String(t.year), 81, 57);
          break;
        case 8:
          myOLED.print(String(t.date), 28, 57);
          myOLED.print(String("CTHGYZ"), CENTER, 57);
          myOLED.print(String(t.year), 90, 57);
          break;
        case 9:
          myOLED.print(String(t.date), 24, 57);
          myOLED.print(String("DTHTCYZ"), CENTER, 57);
          myOLED.print(String(t.year), 94, 57);
          break;
        case 10:
          myOLED.print(String(t.date), 26, 57);
          myOLED.print(String(":JDNYZ"), CENTER, 57);
          myOLED.print(String(t.year), 92, 57);
          break;
        case 11:
          myOLED.print(String(t.date), 28, 57);
          myOLED.print(String("KBCNJGFLF"), CENTER, 57);
          myOLED.print(String(t.year), 90, 57);
          break;
        case 12:
          myOLED.print(String(t.date), 26, 57);
          myOLED.print(String("UHELYZ"), CENTER, 57);
          myOLED.print(String(t.year), 92, 57);
          break;
      }
      myOLED.update();
    }
else{
      frametimeon = false;
      myOLED.setFont(MegaNumbers);
      myOLED.print(String("-"), 51, 12);
      myOLED.update();
    }

 if (nastroika == true) {
   interval = 100;
 }
 if (nastroika == false){
   interval = 500;
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
  int menupos = 0; //0


  x_direction = 0;
  y_direction = 0;

  x_position = analogRead(A2);
  y_position = analogRead(A3);


if (x_position > X_THRESHOLD_HIGH){
    x_direction = 1;
}
else if (x_position < X_THRESHOLD_LOW){
    x_direction = -1;
}
if (y_position > Y_THRESHOLD_HIGH){
    y_direction = 1;
}
else if (y_position < Y_THRESHOLD_LOW){
    y_direction = -1;
}

if (x_direction == -1){
    if (y_direction == -1){
      }
    else if (y_direction == 0){
        menupos++;
        }
}

else if (x_direction == 0){
     if (y_direction == -1){
       if (menu ==0 && nastroika == true){

        znach --;
       }
       if(menu == 1 && nastroika == true){
        znach --;
       }
        if(menu == 2 && nastroika == false){

            zadtemp--;
            EEPROM.update(1,zadtemp);
        }
        if(menu == 1 && nastroika == false){
          myStepper.step(100);
        }
}
else if (y_direction == 0){
          }
else {
     if(menu == 0 && nastroika == true){
         znach ++;
     }
     if(menu == 1 && nastroika == true){
         znach ++;
     }
     if(menu == 2 && nastroika == false){

          zadtemp++;
          EEPROM.update(1,zadtemp);
          }// y_direction == 1
     if(menu == 1 && nastroika == false){
       myStepper.step(-100);
          }
       }
}

else {
    // x_direction == 1
     if (y_direction == -1){
        }
     else if (y_direction == 0){
            menupos--;
             }
}

     if (menupos == 1){
        menu = menu + 1;
                     }
     if (menupos == -1){
        menu = menu - 1;
                       }
     if (menu > 2){
       myOLED.clrScr();
       menu  = 0;
     }
     if (menu < 0){
       menu = 0;
     }
buttonPinstate = digitalRead(buttonPin);
if (y_direction == 0 && x_direction ==0 && buttonPinstate == LOW){
  nastroika = true;
}

}

void  fann(){
Input2 = cel;
Setpoint2 = zadtemp;
myPID2.Compute();
//  digitalWrite(fan,255);// виключений
//digitalWrite(fan,0);// включений
speedfan = Output2;
analogWrite(fan,speedfan);
Serial.println(speedfan);
}

void zagruzka(){
  myOLED.clrScr();
  myOLED.setFont(UkrFont);
  myOLED.print(String("GTHTDSHRF"),  CENTER,  20);
  myOLED.print(String("CBCNTVB"),  CENTER,  40);
  myOLED.update();
}


void salon() {
    myOLED.clrScr();
    myOLED.setFont(UkrFont);
    myOLED.print(String("PFLFYF NTVGTHFNEHF"),  CENTER,  0);
    myOLED.setFont(MegaNumbers);
    myOLED.print(String(zadtemp),  CENTER,  13);
  if(winter == true){
    myOLED.setFont(UkrFont);
    myOLED.print(String("Ghjuhsd Fdnj"),  CENTER,  57);
  }
  if (winter == false) {
    myOLED.setFont(UkrFont);
    myOLED.print(String("Ht;bv Ksnj"),  CENTER,  57);
  }
    myOLED.update();
}


void m(){
  if (menu == 0 && nastroika == false){
  calendar();
  EEPROM.update(2, menu);
  }
 if (menu == 1 && nastroika == false){
  grad();
  EEPROM.update(2, menu);
  }
 if (menu == 2 && nastroika == false){
  salon();
  EEPROM.update(2, menu);
 }
}

void sezon(){


  bootnstate = digitalRead(buttonPin);
  koncpos = digitalRead(koncevik);
  if(cel <=18){
    winter = true;
    leto = false;
    digitalWrite(pomp, HIGH);
  }
  if (cel >=19){
    winter = false;
    leto = true;
    digitalWrite(pomp, LOW);
    }


int stepmotor = Output;
stepmotor = map(stepmotor, 0,2800,0,-2800);
stepC = stepmotor - stepB;
myStepper.step(stepC);
stepB = stepB + stepC;


if (stepB != stepvalueold){
  stepvalueold = stepB;
  digitalWrite( 5, LOW );
  digitalWrite( 4, LOW );
  digitalWrite( 3, LOW );
  digitalWrite( 2, LOW );
  Serial.println(stepvalueold);
}
Serial.println(stepB);
}

void pidreg(){
  Input = cel;
  Output = stepvalue;
  Setpoint = zadtemp;
  myPID.Compute();
  }

void n(){
    if (menu == 0 && nastroika == true){
    calendar();
    houres = true;
    minutes = false;
    t = rtc.getTime();
    hourse = t.hour;
    minute = t.min;
    if (houres == true && minutes == false){
      if (znach >=24){
        znach = 1;
      }
      Serial.println(hourse);
      if (znach != hourse){
        rtc.setTime(znach, minute, 0);
      }
    }
    }
   if (menu == 1 && nastroika == true){
     t = rtc.getTime();
     hourse = t.hour;
     minute = t.min;
     calendar();
     houres  = false;
     minutes = true;
     if (houres == false && minutes == true){
       if (znach >=60){
         znach = 1;
       }
       Serial.println(hourse);
       if (znach != minutes){
         rtc.setTime(hourse, znach, 0);
       }
     }
    }
   if (menu == 2 && nastroika == true){
     calendar();
     houres  = false;
     minutes = false;
     nastroika = false;
   }
  }








void loop() {
  koncpos = digitalRead(koncevik);
  blink();
  if(stepvork == false){
    zagruzka();
    if (koncpos == LOW){
      myStepper.step(200);
      Serial.println(200);
    }
    if (koncpos == HIGH && zaslonkastate == false){
      zaslonkastate = true;
      stepvork = true;
      }
  }
if (stepvork == true && zaslonkastate == true){
  Serial.println();
    dalas();
    dg();
    sezon();
    pidreg();
    fann();
  }
  if(nastroika == false && stepvork == true && zaslonkastate == true){
    m();
  }
  if(nastroika == true && stepvork == true && zaslonkastate == true){
    n();
  }

  //Serial.println(cel);
  //Serial.println(cel2);

}
