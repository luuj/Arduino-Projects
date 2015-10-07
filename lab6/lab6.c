//Name: Jonathan Luu
//Class: EE109

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

void writecommand (unsigned char);
void writeregister (unsigned char);
void nibbleout (unsigned char);	
void name();


void init_lcd()
{
   _delay_ms (15);
   writecommand(0b0011);
   _delay_ms (5);
   writecommand (0b0011);
   _delay_ms (1);
   writecommand (0b0011);
   writecommand (0b0010);
   _delay_ms(2);
   writecommand (0b0010);
   writecommand (0b1000);
   _delay_ms (2);
   writecommand (0b0000);
   writecommand (0b1111);
   _delay_ms (2);
}

void stringout (char *s)
{
   int length = strlen(s);
   int i;
   for (i=0; i<length; i++)
   {
      writeregister (s[i]);
   }
}

void LCDwhere (unsigned char z)
{
   unsigned char t;
   t=0x80 + z;
   writecommand(t);
}

void writecommand(unsigned char x)
{
   PORTB &= ~(1<<PB0);  
   nibbleout (x>>4);
   nibbleout (x);
   _delay_ms(2);
}

void writeregister (unsigned char b)
{
   PORTB |= (1<<PB0);
   nibbleout (b>>4);
   nibbleout (b);
   _delay_ms(2);
}

void nibbleout (unsigned char a)
{
   PORTD |= (a << 4); 
   PORTD &= (a << 4);   
   PORTB |= (1 << PB1);
   PORTB &= ~(1 << PB1);
}


void name ()
{
   unsigned char st;
   char input[16] = {"Jonathan"};

   stringout (input);
   
   char input2[16] = {"Luu"};
   st = 0x40;
   
   LCDwhere (st);
   stringout (input2);
}


void number()
{
    unsigned char x, start;
    char counter=0;
    char buffer [15];
    start = 0;
    
    while (1){
	ADCSRA |= (1 << ADSC);  // Start a conversion
    
	while (ADCSRA & (1 << ADSC)); // wait for conversion complete
        x = ADCH;               // Get converted value
        
    LCDwhere(start);

    if (x >= 0 && x < 16)
      counter -=1;
    else if (x > 100 && x< 165)
      counter +=1;
    else if (x>16 && x<50 )
      counter +=10;
    else if (x>50 && x<100 )
      counter -=10;
    else if (x>165 && x<200)
      counter = 0 ;
        
    sprintf(buffer,"%4d",counter);
    stringout(buffer);
    _delay_ms(100);
    }
}


int main()
{
    DDRD |= 0xFF;
    DDRB |= (1<<PB0 | 1<<PB1);
    
    ADMUX |= (1 << REFS0);      // Set reference to AVCC
    ADMUX &= ~(1 << REFS1);
    
    ADMUX |= (1 << ADLAR);      // Left adjust the output
    
    ADMUX |= (0 << MUX0); // Select the channel
    
    ADCSRA |= (7 << ADPS0);  // Set the prescalar
    ADCSRA &= (0xf8 | (7 << ADPS0));
    
    ADCSRA |= (1 << ADEN);      // Enable the ADC
    
    init_lcd();
    writecommand(1);
    
    unsigned char start;
    
    start = 0; //0x00~0x0F for first line, 0x40~0x4F for second line
    
    LCDwhere (start);
    
    //name(); Function to output name
    
    number(); //Function to display number
    return 0;
}