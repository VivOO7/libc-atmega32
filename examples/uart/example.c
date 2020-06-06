#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"

void main(void)
{
	uartBegin(9600,'N',1);
	
 	while(1)
	{
		if( uartRead() == 's' ) uartSend('a');
	}
}

