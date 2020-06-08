#include <avr/io.h>
#include <avr/interrupt.h>

#define _SLEEP() __asm__ __volatile__ ( "sleep" "\n\t" :: )
#define _NOP() __asm__ __volatile__ ( "nop" "\n\t" :: )

void pinMode(unsigned char mode, unsigned char port, unsigned char pin)
{
	if(pin <= 7 )
	{
		switch( mode )
		{
			case 'i' :
			case 'I' :
			// pin to be set as input
				if( port == 'A')
				{
					PORTA &= (~(1<<pin));
					DDRA &= (~(1<<pin));
				}
				else if(port == 'B')
				{
					PORTB &= (~(1<<pin));
					DDRB &= (~(1<<pin));
				}
				else if(port == 'C')
				{
					PORTC &= (~(1<<pin));
					DDRC &= (~(1<<pin));
				}
				else if(port == 'D')
				{
					PORTD &= (~(1<<pin));
					DDRD &= (~(1<<pin));
				}
			break;
		
			case 'o' :
			case 'O' :
			// set the pin as output
				if( port == 'A')
				{
					PORTA &= (~(1<<pin));
					DDRA |= (1<<pin);
				}
				else if(port == 'B')
				{
					PORTB &= (~(1<<pin));
					DDRB |= (1<<pin);   
				}
				else if(port == 'C')
				{
					PORTC &= (~(1<<pin));
					DDRC |= (1<<pin); 
				}
				else if(port == 'D')
				{
					PORTD &= (~(1<<pin));
					DDRD |= (1<<pin);
				}
			break;
			
			case 'p' :
			case 'P' :
			// set the pin as input with pull up enabled
				if( port == 'A')
				{
					PORTA |= (1<<pin);
					DDRA &= (~(1<<pin));
				}
				else if(port == 'B')
				{
					PORTB |= (1<<pin);
					DDRB &= (~(1<<pin));
				}
				else if(pp == 'C')
				{
					PORTC |= (1<<sft);
					DDRC &= (~(1<<sft));
				}
				else if(pp== 'D')
				{
					PORTD |= (1<<sft);
					DDRD &= (~(1<<sft));
				}
			
			break;
			
		}
	}	
}	


void setState(unsigned char state, unsigned char port, unsigned char pin)
{
	if(pin <= 7 )
	{
		unsigned char PORT;
		switch( port )
		{
			case 'A' :
			if( DDRA & (1<<pin) )
			{
				if(state)
					PORTA |= (1<<pin);
				else
					PORTA &= (~(1<<pin));
			}
			break;
			
			case 'B' :
			if( DDRB & (1<<pin) )
			{
				if(state)
					PORTB |= (1<<pin);
				else
					PORTB &= (~(1<<pin));
			}
			break;

			case 'C':
			if( DDRC & (1<<pin) )
			{
				if(state)
					PORTC |= (1<<pin);
				else
					PORTC &= (~(1<<pin));
			}
			break;

			case 'D':
			if( DDRD & (1<<pin) )
			{
				if(state)
					PORTD |= (1<<pin);
				else
					PORTD &= (~(1<<pin));
			}
			break;
			
		}
	}	
}


char getState(unsigned char port, unsigned char pin)
{
	if(pin <= 7 && (port=='A' || port=='B' || port=='C' || port=='D') )
	{
		unsigned char PORT;
		unsigned char input;
		switch( port )
		{
			case 'A' :
			PORT=PORTA;
			PORTA=PORT;
			_NOP();
			input=PINA;
			break;
			
			case 'B' :
			PORT=PORTB;
			PORTB=PORT;
			_NOP();
			input=PINB;
			break;

			case 'C':
			PORT=PORTC;
			PORTC=PORT;
			_NOP();
			input=PINC;
			break;

			case 'D':
			PORT=PORTD;
			PORTD=PORT;
			_NOP();
			input=PIND;
			break;
			
		}
		if( input & (1<<pin) ) return 1;
		else return 0;
	}	
	else
		return -1;
}

