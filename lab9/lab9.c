//Jonathan Luu
//EE109 - Lab 9a

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>

int counter = 23;
uint8_t pA=0, pB=0, state, oldstate = 0;
void init_adc();

void pwm_init (void)
{
  TCCR0A |= (0b11 << WGM00 ); // Fast PWM mode , modulus = 256
  TCCR0A |= (0b10 << COM0A0 ); // Turn D6 on at 0 x00 and off at OCR0A
  OCR0A = 23; // Initial pulse width of 1.5 msec
  TCCR0B |= (0b101 << CS00 ); // Prescaler = 1024 = > 16 us period
}

//0.75 ms = 11.7 count value
//2.25 ms = 35.2 count value
// 35.2 - 11.7 = 23.5
// 23.5 / 64 = 0.37 revolutions from min to max
  
int main ()
{
  init_adc();
  
  DDRD |= (1<<PD6); //Makes Port D, bit 6 a PWM pulse output
  pwm_init();
   
  PCICR |= (1 << PCIE1);  //Setting mask register and enabling pin interrupts
  PCMSK1 |= (1 << PC4 | 1 << PC5);
  sei();
  
  while (1)
  {
    if (counter > 35)
    {
      cli();
      counter = 35;
      sei();
    }
    if (counter < 12)
    {
      cli();
      counter = 12;
      sei();
    }
    OCR0A = counter;    
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
