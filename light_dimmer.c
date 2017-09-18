/*Copyright (c) <2017>, <Rachot Phuengsuk>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the <organization>.
4. Neither the name of the <organization> nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define PWM1   3
#define PWM2   10


int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor 

    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result

    if (adc_key_in > 1000) return btnNONE; 

    // For V1.1 us this threshold
    /*if (adc_key_in < 50)   return btnRIGHT;  
    if (adc_key_in < 250)  return btnUP; 
    if (adc_key_in < 450)  return btnDOWN; 
    if (adc_key_in < 650)  return btnLEFT; 
    if (adc_key_in < 850)  return btnSELECT;  */

   // For V1.0 comment the other threshold and use the one below:
   
     if (adc_key_in < 50)   return btnRIGHT;  
     if (adc_key_in < 195)  return btnUP; 
     if (adc_key_in < 380)  return btnDOWN; 
     if (adc_key_in < 555)  return btnLEFT; 
     if (adc_key_in < 790)  return btnSELECT;   
   

    return btnNONE;                // when all others fail, return this.
}

int percent_output1=0;
int percent_output2=0;
int percent_output1_old;
int percent_output2_old;
int mode=0;
int mode_timeout=50;
int time_count=0;


void setup() 
{
  Serial.begin(9600);
   lcd.begin(16, 2);               // start the library
   pinMode(PWM1, OUTPUT);
   pinMode(PWM2, OUTPUT);
}

void lcd_display()
{
   if(mode == 0)
   {
     lcd.setCursor(0,0);             
     lcd.print("  R&B  INVERTER ");       
     lcd.setCursor(0,1);                
     lcd.print("  R: ");               
     lcd.print(percent_output1);
     lcd.print("%");
     lcd.print(" B: ");               
     lcd.print(percent_output2);
     lcd.print("%     ");
   }
   else if(mode == 1)
   {
    lcd.setCursor(0,0);             
     lcd.print("      MODE1     ");      
     lcd.setCursor(0,1);               
     lcd.print("    RED = ");                
     lcd.print(percent_output1);
     lcd.print("%    ");   
   }
   else if(mode == 2)
   {
     lcd.setCursor(0,0);             
     lcd.print("      MODE2     ");      
     lcd.setCursor(0,1);                
     lcd.print("    BLUE = ");            
     lcd.print(percent_output2);
     lcd.print("%    ");    
   }   
}
void loop() 
{
   lcd_key = read_LCD_buttons();       // read the buttons
   lcd_display();
   
    switch (lcd_key)   // depending on which button was pushed, we perform an action
   {             

       case btnRIGHT:{             //  push button "RIGHT" and show the word on the screen
             mode = 2;
             percent_output1_old = percent_output1;
             delay(50);
            break;
       }
       case btnLEFT:{
             mode = 1;
             percent_output2_old = percent_output2;
             delay(50);
             break;
       }    
       case btnUP:{
            time_count=0;
            if(mode == 1)
            {
              percent_output1++;
              if(percent_output1>100)
                percent_output1 = 100;
              delay(50);
            }
            else if(mode == 2)
            {
              percent_output2++;
              if(percent_output2>100)
                percent_output2 = 100;
              delay(50);
            }
             break;
       }
       case btnDOWN:{
            time_count=0;
            if(mode == 1)
            {
              percent_output1--;
              if(percent_output1<0)
                percent_output1 = 0;
              delay(50);
            }
            else if(mode == 2)
            {
              percent_output2--;
              if(percent_output2<0)
                percent_output2 = 0;
              delay(50);
            }
            
             break;
       }
       case btnSELECT:{
             if(mode == 1)
             {
                analogWrite(PWM1, percent_output1*2.55);
                percent_output1_old = percent_output1; 
             }
             if(mode == 2)
             {
                analogWrite(PWM2, percent_output2*2.55);
                percent_output2_old = percent_output2;
             }

             mode = 0;
             break;
       }
       case btnNONE:{

             break;
       }
   }// switch (lcd_key)

  time_count++;
  if(time_count == mode_timeout) // time out
  {
    time_count = 0;
    mode = 0;
    percent_output1 = percent_output1_old;
    percent_output2 = percent_output2_old;
  }
  delay(100);

}
