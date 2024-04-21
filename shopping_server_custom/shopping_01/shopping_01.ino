#include "HX711.h"
//#include <stdio.h>
//#include <stdlib.h>

#include<LiquidCrystal.h>
#include <Keypad.h>

#define Password_Length 11

#define LOADCELL_DOUT_PIN A4
#define LOADCELL_SCK_PIN  A5

#define calibration_factor 200
 
const int rs = 9, en = 8, d4 = 13, d5 = 12, d6 = 11, d7 = 10;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char keybuf[11];

char customKey=0;

HX711 scale;
float units;
String card;

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', 'A', 'B'},
  {'C', 'D', 'E', 'F'}
};

byte rowPins[ROWS] = {14,15,16,17}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2,3,4,5}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


int m=0,l=1;

void setup()
{
 
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" SMART SHOPPING");
  lcd.setCursor(0,1);
  lcd.print("TROLLEY SYSTEM");
  delay(1000);
}
void loop()
{
  PASSWARD();


}


void PASSWARD()
{ 
          l=1;
          m=0;   
          while(l)
         {
        
            lcd.clear();
            lcd.print("ENTER YOUR  ");
            lcd.setCursor(0,1);
            lcd.print("MOBILE NUMBER:");
//            Serial.println("ENTER YOUR MOBILE NUMBER");
            while(m<10)
            {
              customKey = customKeypad.getKey();
              if(customKey)
              {
                lcd.setCursor(15,1);
                lcd.print(customKey);
//                 Serial.print(customKey);
                 keybuf[m++]=customKey;
              }
            }
            
            keybuf[10]=0;  
            l=0;
            m=0;
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("MOBILE  NUMBER:");
            lcd.setCursor(0,1);
            lcd.print(keybuf);
            Serial.println("Phone Num:"+String( keybuf ));
            delay(2000);
          

       }
       serial_check();
}
void Weight()
{
 scale.set_scale(calibration_factor); //Adjust to this calibration factor
 units=scale.get_units()/10;
 if(units <0)
 {
  units=0.0;
   
 }
  Serial.print("Weight: ");
  Serial.print(units);
  Serial.println("gms"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person

  Serial.println();

}
void serial_check()
{ 
  lcd.clear();
  lcd.print("SELECT PRODUCT");
  delay(1000);
  
  while(1){
  if(customKey = customKeypad.getKey()=='1')
  {
    Serial.println('E');
    break;
  }
  if(Serial.available()>0)
  {
    card=Serial.readString();
    Serial.println(card);
    Weight();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(card);
    lcd.setCursor(0,1);
    lcd.print(units);
    delay(1000);
  }
  
//  break;
  }
  
}
