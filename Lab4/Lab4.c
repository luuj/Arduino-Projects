// Jonathan Luu EE109 - Lab 4 text

#include <avr/io.h>

int main(void)
{
    unsigned char test;
    
    DDRD |= (1 << PD1 | 1 << PD2 | 1<< PD3 | 1<< PD4 | 1 << PD5 | 1<<PD6 | 1<<PD7); // Setting pins to output
    
    ADMUX &= ~(1 << MUX0 | 1 << MUX1 | 1 << MUX2 | 1 << MUX3); //Input channel
    
    ADMUX |= (1<< ADLAR); // Left adjust for 8 bit
    
    ADMUX |= ~(1 << REFS1); // Connect to AVCC
    ADMUX |= (1 << REFS0);
    
    ADCSRA |= (1 << ADPS2 | 1 << ADPS1); // Pre-scaler
    ADCSRA &= ~(1 << ADPS0);
    
    ADCSRA |= (1 << ADEN); //ADC Enable
    
    
    while (1)
    {
    ADCSRA |= (1 << ADSC); // Start conversion
    
      while (ADSC == 1)
      {
      }
      
    test = ADCH;
    
    // Calculating display number
    
    if (test >=0 && test <=25)
    { 
       PORTD &= ~(1 << PD1 | 1 << PD2 | 1 << PD3 | 1 << PD4 | 1 << PD5 | 1 << PD6);
       PORTD |= (1 << PD7);
    }
    if (test >25 && test <=51)
    { 
       PORTD &= ~(1 << PD2 || 1 << PD3); 
       PORTD |= (1 << PD7 | 1  << PD1 |1 << PD4|1 << PD5|1 << PD6); 
    }
    if (test >51 && test <=76)
    { 
       PORTD &= ~(1<<PD1 | 1<<PD2 | 1<<PD7 | 1<<PD5 | 1<<PD4); 
       PORTD |= (1 << PD3|1 << PD6);
    } 
    if (test >76 && test <=102)
    {
       PORTD &= ~(1<<PD1 | 1<<PD2 | 1<<PD7 | 1<<PD3 | 1<<PD4);
       PORTD |= (1 << PD5|1 << PD6);
    }
    if (test >102 && test <=128)
    { 
       PORTD &= ~(1<<PD6 | 1<<PD7 | 1<<PD2 | 1<<PD3);
       PORTD |= (1 << PD1|1 << PD4|1 << PD5);
    }
    if (test >128 && test <=153)
    { 
       PORTD &= ~(1<<PD1 | 1<<PD6 | 1<<PD7 | 1<<PD3 | 1<<PD4);
       PORTD |= (1 << PD2|1 << PD5);
    }
    if (test >153 && test <=179)
    { 
       PORTD &= ~(1<<PD1 | 1<<PD6 | 1<<PD5 | 1<<PD4 | 1<<PD3 | 1<<PD7);
       PORTD |= (1 << PD2);
    }   
    if (test >179 && test <=204)
    { 
       PORTD &= ~(1<<PD1 | 1<<PD2 | 1<<PD3);
       PORTD |= (1 << PD4|1 << PD5|1 << PD6|1 << PD7);
    }
    if (test >204 && test <=230)
    { 
       PORTD &= ~(1<<PD1 | 1<<PD2 | 1<<PD3 | 1<<PD4 | 1<<PD5 | 1<<PD6 | 1<<PD7);
    }
    if (test >230 && test <=256)
    { 
       PORTD &= ~(1<<PD1 | 1<<PD2 | 1<<PD3 | 1<<PD7 | 1<<PD6);
       PORTD |= (1 << PD4|1 << PD5);
    }
    
  }
        return 0;
}
    