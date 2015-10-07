//Jonathan Luu
//EE109 - Lab 8a

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>

void writecommand(unsigned char);
void writeregister(unsigned char);
void nibbleout(unsigned char);
void init_lcd();
void init_adc();
void stringout (char* s);
void LCDwhere (unsigned char);

int counter = 0;
uint8_t pA=0, pB=0, state, oldstate = 0;
  
void spinner() //Program 8A
{
  char buff[4];
  
  while (1)  
  {
    pB = PINC & (1<<PD5); //Reading input from PC5 and PC4
    pA = PINC & (1<<PD4);
    
    if (pB == 32 && pA == 00)
      state = 1;
    if (pB == 0 && pA == 0)
      state = 2;
    if (pB == 0 && pA == 16)
      state = 3;
    if (pB == 32 && pA == 16)
      state = 4;      
  
    if (oldstate != state) //Only change output if wheel is turned
    {
      sprintf(buff, "%4d", counter);
      LCDwhere(0x09);
      stringout(buff);
      
      if (state == 1)
      {
        if (oldstate == 4)
          counter++;
        if (oldstate == 2)
          counter--;
      }    
      
      if (state == 2)
      {
        if (oldstate == 1)
          counter++;
        if (oldstate == 3)
          counter--;
      }
      
      if (state == 3)
      {
        if (oldstate == 2)
          counter++;
        if (oldstate == 4)
          counter--;
      }        
      
      if (state == 4)
      {
        if (oldstate == 3)
          counter++;
        if (oldstate == 1)
          counter--;
      }            
      oldstate = state;
    }   
  }    
}
    
int main ()
{
  init_lcd();
  init_adc();
  
  LCDwhere(0); //Clear screen
  writecommand(1);
  
  LCDwhere(0x01);
  char message[14] = "Counter:";
  stringout(message);

  char buffer[4];
  
  while (1)
  {
      spinner();
      sprintf(buffer, "%4d", counter);
      LCDwhere(0x09);
      stringout(buffer);
      _delay_ms (50);  
  } 
  
}
