#include <avr/io.h>
#include <avr/interrupt.h>

#define _SLEEP() __asm__ __volatile__("sleep" "\n\t" ::)

void sleepMode(uint8_t mode)
{
	if(mode<=0 || mode>=7 || mode==4 || mode==5)
		return;
	
	unsigned char MODE=0X00;
	
	MODE = ( mode<<4 );
	MODE |= ( ( mode<<5 ) & 0X80 );
	MODE &= 0XB0;
	MODE |= (MCUCR & 0X0F);

	MCUCR = MODE;
}

void gotoSleep(void)
{
	MCUCR |= (1<<SE);
	sei();
	_SLEEP();
	//statement to execute after wakeup routine
	MCUCR &= (~(1<<SE));
}
