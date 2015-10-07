#include <avr/io.h>
#include <avr/interrupt.h>

void tx_char(char ch);
void powerOff();
void powerOn();
void zoomUp();
void zoomDown();

char orderArrCW [] = {1,3,0,2};
char orderArrCCW [] = {2,0,3,1};
volatile int state, powerState = 0;
int lastState = 0;

int main()
{
  DDRD |= (1<<PD1); //Setting to output
  UBRR0 = 51; //Initialization code
  UCSR0B |= (1 << TXEN0 | 1 << RXEN0);
  UCSR0C = (3 << UCSZ00);
  
  PCICR |= (1<<PCIE1); //Rotary encoder initialization
  PCMSK1 |= (1<<PC4 | 1 << PC5);
  sei();
  
  DDRC |= (1 <<PC4 | 1<<PC5); 
  PORTC |= (1<<PC4 |1 << PC5 | 1 <<PC3);
  
  while (1)
  {
    int c = ((PINC & (1<<PC3)) >> PC3);
    
    if (c == 0 && powerState == 0)
    {
      powerOn();
      powerState = 1;
    }
    
    if (c == 0 && powerState == 1)
    {
      powerOff();
      powerState = 0;
    }       
  }
  
  return 0;
}


void tx_char (char ch)
{
  while ((UCSR0A & (1<<UDRE0)) == 0);
  UDR0 = ch;
}

void sendChar(char x, char y, char z)
{
  tx_char(0x02);
  tx_char(x);
  tx_char(y);
  tx_char(z);
  tx_char(0x03);
}  

void powerOff (){
  sendChar('P', 'O', 'F');
}

void powerOn(){
  sendChar('P', 'O', 'N');
}

void zoomUp(){
  sendChar('D', 'Z', 'U');
}

void zoomDown(){
  sendChar('D', 'Z', 'D');
}  

ISR(PCINT1_vect)
{
  int a = ((PINC & (1<<PC4)) >> PC4);
  int b = ((PINC & (1<<PC5)) >> PC5);
  state = a+(2*b);
  
  if (state == orderArrCW[lastState])
  {
    zoomUp();
  }
  else if (state == orderArrCCW[lastState])
  {
    zoomDown();
  }
  lastState = state;
}
  
