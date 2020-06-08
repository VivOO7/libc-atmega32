#include <avr/interrupt.h>
#include <util/delay.h>
#include "io.h"

void main(void)
{
	pinMode('D',7,'o');
	
	while(1);
	{
		setState('D',7,1);
		_delay_ms(1000);
		setState('D',7,0);
		_delay_ms(1000);
	}
}
