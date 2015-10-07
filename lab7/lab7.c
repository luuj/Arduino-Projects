//Jonathan Luu
//EE109 - Lab 7

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define START   1
#define STOP	0

void writecommand(unsigned char);
void writeregister(unsigned char);
void nibbleout(unsigned char);

uint8_t time[3];
uint8_t adc_result;
uint8_t state = 0;
uint8_t state2 = 0;
	
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
    
    ADMUX |= (1 << REFS0);      // Set reference to AVCC
    ADMUX &= ~(1 << REFS1);
    
    ADMUX |= (1 << ADLAR);      // Left adjust the output
    
    ADMUX |= (0 << MUX0); // Select the channel
    
    ADCSRA |= (7 << ADPS0);  // Set the prescalar
    ADCSRA |= (1 << ADEN | 1 << ADIE);      // Enable the ADC and interrupts   
}

void init_timer()	
{
	TCCR1B |= (1<<WGM12); //CTC mode
    TIMSK1 |= (1 <<OCIE1A); // Enable interrupt
    
    OCR1A = 25000;
    TCCR1B |= (0b011 << CS10); //Pre-scaler of 64
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

void startstop()	//Start and stop watch function
{
	while (1)
	{
	  _delay_ms(200);
      ADCSRA |= (1 << ADSC);  // Start a conversion
     	
      while (ADCSRA & (1 << ADSC)); // wait for conversion complete
      adc_result = ADCH;               // Get converted value
      
	  if(adc_result > 16 && adc_result <50 && state==STOP) //START Timer
	  {
	     sei();
	     state +=1;  //Change state to started
	     _delay_ms(40);
	     
	     if (state2 == 1)	//State = Start/Stop state | State2 = Lap/Reset state
	        state2--;
	        
	     break;
	  }
	  
	  if(adc_result > 16 && adc_result <50 && state==START) //STOP Timer
      {
 	     cli();
	     state -=1; //Change state to stopped
	     _delay_ms(40);
	     break;
	  }
	  
	  if(adc_result > 50 && adc_result < 100 && state==STOP) //LAP Reset
	  {
	     if (state2 ==0)
	     {
	        char entry[15]; //Reset time array back to 00.0
	   	    time[0]=0;
	 	    time[1]=0;
	  	    time[2]=0;
	  	
	  	    LCDwhere (0x46);
	  	    sprintf(entry,"%d%d.%d",time[2],time[1],time[0]);
	        stringout(entry);
	  	    break;
	  	 }
	  }
	  
	  if(adc_result >50 && adc_result <100 && state==START) //LAP feature
	  {	  
		 if (state2==0)
		 {
	        char entry1[10];	//Save current time in separate array + pause
	        entry1[0]=time[2]+'0';
	        entry1[1]=time[1]+'0';
	        entry1[2]='.';
	        entry1[3]=time[0]+'0';	
	     
	        LCDwhere (0x46);
	        stringout(entry1);
	        state2 +=1;
	        state -=1;
	        break;
	     }
	   }
	   
	   if (adc_result >50 && adc_result <100 && state2==1 && state==STOP)  
	   {
	        state2 -=1;	// Continue if lap/reset is pressed again
	        state +=1;
	        break;
	   }     
	    
   }        
}
	   
int main()
{
	init_lcd();
	init_adc();
	init_timer();
	
	LCDwhere (0x00); //Clearing display
	writecommand(1);
	
	LCDwhere(0x02);		
	char message[14] = "My Stopwatch";
	stringout(message);
	
	LCDwhere(0x46);
	char starttime[5] = "00.0";
	stringout(starttime);
    	
	while (1)
	{
	   startstop();
	}	
}

ISR (ADC_vect)
{
	adc_result = ADCH;
}

ISR(TIMER1_COMPA_vect)
{
	char buffer[15];
	time[0] +=1;
	
	if (time[0] == 10) //Incrementing each time value separately
	{
		time[0] = 0;
		time[1] +=1;
	}	
	if (time[1] == 10)
	{
		time[1] = 0;
		time[2] +=1;
	}	
	
	if (time[2] == 5 && time[1] == 9 && time [0] == 9) //Stop if time > 59.9 sec
		state = STOP;
		
	if (state == START)
	{
	   LCDwhere (0x46);
	   sprintf(buffer,"%d%d.%d",time[2],time[1],time[0]);
	   stringout(buffer);
	}
}
