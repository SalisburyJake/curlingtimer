#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();
char displaybuffer[4] = {' ', ' ', ' ', ' '};


void displayInit()
{
  alpha4.begin(0x70);  // pass in the address  
  alpha4.clear();
  alpha4.writeDisplay();
}


void display_WriteFinalTime(float l_time_f)
{ 
  dtostrf(l_time_f, 4, 2, displaybuffer); 
  sendToDisplay();
}

void displayWriteSet()
{
  displaybuffer[0] = 'T';
  displaybuffer[1] = 'S';
  displaybuffer[2] = 'E';
  displaybuffer[3] = 'T';
  sendToDisplay();
}

void sendToDisplay()
{
  alpha4.writeDigitAscii(0, displaybuffer[0]);
  alpha4.writeDigitAscii(1, displaybuffer[1]);
  alpha4.writeDigitAscii(2, displaybuffer[2]);
  alpha4.writeDigitAscii(3, displaybuffer[3]);
  alpha4.writeDisplay();
  delay(200);
}
