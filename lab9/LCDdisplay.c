//Jonathan Luu

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

void init_lcd()  //Initialize the LCD screen
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

void init_adc()	//Initialize the ADC
{
    DDRD |= 0xFF;
    DDRB |= (1<<PB0 | 1<<PB1);
    DDRC &= ~(1<<PB4 | 1 <<PB5); //Input pins
    PORTC |= (1 << 4 | 1 << 5);  //Enabling pull-up resistors
        
    ADMUX |= (1 << REFS0);      // Set reference to AVCC
    ADMUX &= ~(1 << REFS1);
    
    ADMUX |= (1 << ADLAR);      // Left adjust the output
    
    ADMUX |= (0 << MUX0); // Select the channel
    
    ADCSRA |= (7 << ADPS0);  // Set the prescalar
    ADCSRA |= (1 << ADEN | 1 << ADIE);      // Enable the ADC and interrupts   
}

void stringout (char* s)  //Print out given string
{
   char buff;
   while (1)
   {
      buff = *s;
      if (buff == 0)
      	 break;
      writeregister(buff);
      s++;	
   }
}
      
void LCDwhere (unsigned char z) {
   unsigned char t;
   t=0x80 + z;
   writecommand(t); }
   
void writecommand(unsigned char x) {
   PORTB &= ~(1<<PB0);  
   nibbleout (x>>4);
   nibbleout (x);
   _delay_ms(2); }
   
void writeregister (unsigned char b) {
   PORTB |= (1<<PB0);
   nibbleout (b>>4);
   nibbleout (b);
   _delay_ms(2); }
   
void nibbleout (unsigned char a) {
   PORTD |= (a << 4); 
   PORTD &= (a << 4);   
   PORTB |= (1 << PB1);
   PORTB &= ~(1 << PB1); }