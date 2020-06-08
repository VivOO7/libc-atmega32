#include <avr/io.h>
#include <avr/interrupt.h>


void enableInterrupt(unsigned char pin, unsigned char mode)
{
	uint8_t flag=0;
	switch(pin)
	{
		case 0:
		if(mode=='l' || mode=='L')
		{
			MCUCR &= (~( (1<<ISC01) | (1<<ISC00)));
			flag=1;	
		}
		else if(mode=='t' || mode=='T')
		{
			MCUCR &= (~(1<<ISC01));
			MCUCR |= (1<<ISC00);
			flag=1;
		}
		else if(mode=='f' || mode=='F')
		{
			MCUCR &= (~(1<<ISC00));
			MCUCR |= (1<<ISC01);
			flag=1;
		}
		else if(mode=='r' || mode=='R')
		{
			MCUCR &= ((1<<ISC01) | (1<<ISC00));
			flag=1;
		}
		if(flag)
			GICR |= (1<<INT0);
		break;

		case 1:
		if(mode=='l' || mode=='L')
		{
			MCUCR &= (~( (1<<ISC11) | (1<<ISC10)));	
			flag=1;
		}
		else if(mode=='t' || mode=='T')
		{
			MCUCR &= (~(1<<ISC11));
			MCUCR |= (1<<ISC10);
			flag=1;
		}
		else if(mode=='f' || mode=='F')
		{
			MCUCR &= (~(1<<ISC10));
			MCUCR |= (1<<ISC11);
			flag=1;
		}
		else if(mode=='r' || mode=='R')
		{
			MCUCR &= ((1<<ISC11) | (1<<ISC10));
			flag=1;
		}
		if(flag)
			GICR |= (1<<INT1);
		break;

		case 2:
		if( mode=='f' || mode=='F')
		{
			MCUCSR &= (~(1<<ISC2));
			flag=1;
		}
		else if(mode=='r' || mode=='R')
		{
			MCUCSR |= (1<<ISC2);
			flag=1;
		}
		if(flag)
			GICR |= (1<<INT2);
		break;
	}
	if(flag)
		sei();
}


void disableInterrupt(unsigned char pin)
{
	switch(pin)
	{
		case 0:
		GICR &= (~(1<<INT0));
		break;

		case 1:
		GICR &= (~(1<<INT1));
		break;

		case 2:
		GICR &= (~(1<<INT2));
		break;
	}
}
