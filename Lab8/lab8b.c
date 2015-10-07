//Jonathan Luu
//EE109 - Lab 8

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

volatile int counter = 0;
volatile uint8_t pA=0, pB=0, state, oldstate = 0;
char buffer[4];
  
int main ()
{
  init_lcd();
  init_adc();
  
  LCDwhere(0); //Clear screen
  writecommand(1);
  
  LCDwhere(0x01);
  char message[14] = "Counter:";
  stringout(message);
  
  PCICR |= (1 << PCIE1);  //Setting mask register and enabling pin interrupts
  PCMSK1 |= (1 << PC4 | 1 << PC5);
  sei();
  
  while (1)
  {
  } 
  
}

ISR (PCINT1_vect) //Program 8B
{   
    pB = PINC & (1<<PD5); //Reading input from PC5 and PC4
    pA = PINC & (1<<PD4);
    
    if (pB == 32 && pA == 00)
      state = 1;
    else if (pB == 0 && pA == 0)
      state = 2;
    else if (pB == 0 && pA == 16)
      state = 3;
    else if (pB == 32 && pA == 16)
      state = 4;      
      
      if (state == 1)
      {
        if (oldstate == 4)
          counter++;
        else if (oldstate == 2)
          counter--;
      }    
      
      else if (state == 2)
      {
        if (oldstate == 1)
          counter++;
        else if (oldstate == 3)
          counter--;
      }
      
      else if (state == 3)
      {
        if (oldstate == 2)
          counter++;
        else if (oldstate == 4)
          counter--;
      }        
      
      else if (state == 4)
      {
        if (oldstate == 3)
          counter++;
        else if (oldstate == 1)
          counter--;
      }            
      oldstate = state;
      sprintf(buffer, "%4d", counter);
      LCDwhere(0x09);
      stringout(buffer); 
}   
