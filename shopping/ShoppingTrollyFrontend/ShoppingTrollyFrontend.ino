#include <HX711.h>
//#include <stdio.h>
//#include <stdlib.h>

#include<LiquidCrystal.h>
#include <Keypad.h>
#include"String.h"
#include <SoftwareSerial.h>


char Serial_read(void)
{
      char ch;
      while(Serial.available() == 0);
      ch = Serial.read(); 
      return ch;
}

#define Password_Length 11

#define LOADCELL_DOUT_PIN A4
#define LOADCELL_SCK_PIN  A5

#define calibration_factor 200
 
const int rs = 9, en = 8, d4 = 13, d5 = 12, d6 = 11, d7 = 10;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

char keybuf[11];

char ch;
char Start_buff[100];
char money[100];
char phone[100];

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

// char Start_buff[100];
int m=0,l=1;
SoftwareSerial GSMSerial(6,7);  //GSM TX pin to 5 and RX pin to 6
void setup()
{
 
  Serial.begin(9600);
  GSMSerial.begin(9600);
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

void SendMessage()
{
  GSMSerial.println("AT+CMGF=1");                                 //Sets the GSM Module in Text Mode
  delay(1000);                                                    // Delay of 1000 milli seconds or 1 second
  GSMSerial.println("AT+CMGS=\"+91+String(phone)\"\r");               // Replace x with mobile number
  delay(1000);
  GSMSerial.println(String("MRM Mart\n")+money+String("\n")+String("htttps://www.mrmmart.com/bill/56"));  // The SMS text you want to send
  delay(100);
   GSMSerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);

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
      while(1){
       
   //  while(1){
      if (Serial.available() > 0)
      {
         //Serial.println("halo");

          while(Serial_read()!='@');
    int i=0;
         int ii=0;
         int iii=0;
          char ch;
            while((ch=Serial_read())!='#')
          {
            Start_buff[i] = ch;
            if(i>=0 && i<10){
              phone[ii]=Start_buff[i];
              ii++;
            }

             if(i>10 && i<50){
              money[iii]=Start_buff[i];
              iii++;
            }


             i++;
          }  
          phone[ii]='\0';
           money[iii]='\0';
          Start_buff[i]='\0';

         
          
       }
      //   lcd.clear();
      //     lcd.setCursor(0,1);
      //       lcd.print(Start_buff);
      //       delay(2000);
      //  Serial.println(Start_buff);
  ////}

   // break;
  }
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
