//Jonathan Luu
//EE109 - Lab 9b

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>

int counter = 94;
uint8_t pA=0, pB=0, state, oldstate = 0;
void init_adc();

void pwm_init (void)
{
  TCCR1A |= (0b11 << WGM10 ); 
  TCCR1B |= (0b11 << WGM12);
  TCCR1A |= (1 << COM1B1 ); // Turn D6 on at 0 x00 and off at OCR0A
  OCR1A = 1250;
  OCR1B = 94;
  TCCR1B |= (0b100 << CS00 ); // Prescaler = 64
  
}

  
int main ()
{
  init_adc();
  
  DDRB |= (1<<PB2);
  pwm_init();
   
  PCICR |= (1 << PCIE1);  //Setting mask register and enabling pin interrupts
  PCMSK1 |= (1 << PC4 | 1 << PC5);
  sei();
  
  while (1)
  {
    if (counter > 140)
    {
      cli();
      counter = 140;
      sei();
    }
    if (counter < 47)
    {
      cli();
      counter = 47;
      sei();
    }
    
    OCR1B = counter;    
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
       
}   
