#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ADC_CHAN  0             // Select channel A0
#define ADC_PRSC  7             // Set the prescalar to divide by 128
                                // 16Mhz/128 = 125kHz or 8usec/cycle
                                // Conversions take 13 cycles or 104usec
uint8_t adc_result;             // result of ADC conversion
uint8_t old_result;

int main(void) {

    DDRD |= (1 << PD7);         // Set PORTD bit 7 for output (D7)
	DDRB |= (1 << PB5);
    /* Don't need to clear the zero bits since reset does that */
    // REFS[1:0] = 00 for AREF input, connected to 3.3V
    ADMUX |= (1 << ADLAR);      // Left adjust the output
    ADMUX |= (ADC_CHAN << MUX0); // Select the channel

    DIDR0 |= (1 << ADC_CHAN);   // Disable the digital input buffer
                                // on the ADC channel to save power

    ADCSRA |= (ADC_PRSC << ADPS0);  // Set the prescalar
    ADCSRA |= (1 << ADEN | 1 << ADIE);      // Enable the ADC and interrupts
    
    old_result=25;
    
    TCCR1B |= (1<<WGM12); //CTC mode
    TIMSK1 |= (1<<OCIE1A); // Enable interrupt
    
    OCR1A = (old_result << 6);
    
    TCCR1B |= (0b101 << CS10);
    
    sei ();   // Enable global interrupts
    adc_result = 1;
    
    while (1) {                 // Loop forever
        PORTB ^= (1 << PB5);
        _delay_ms(250);
    }

    return 0;   /* never reached */
}

ISR (ADC_vect)
{
	adc_result = ADCH;
}

ISR(TIMER1_COMPA_vect)
{
	uint16_t new_modulus;
	
	PORTD ^= (1<<PD7);
	
	if (adc_result != old_result)
	{
		if (adc_result == 0)
			new_modulus = (1 << 6);
		else
			new_modulus = adc_result << 6;
			
			OCR1A = new_modulus;
			old_result=adc_result;	
	}
	ADCSRA |= (1 << ADSC); //Starts conversion again
	
}