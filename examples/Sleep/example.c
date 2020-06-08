#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "io.h"
#include "externInterrupt.h"
#include "Sleep.h"

//using external interrupt 0 to wake up the mcu
ISR(INT0_vect)
{
	setState('D',7,0);
	disableInterrupt(0);
}

void main(void)
{
	pinMode('D',7,'o');
	pinMode('D',2,'i');

	sleepMode(2);

	while(1)
	{
		_delay_ms(2000);
		enableInterrupt(0,'L');
		setState('D',7,1);
		gotoSleep();
	}
	
}
