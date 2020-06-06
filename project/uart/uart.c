#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD_PRESCALER (((F_CPU/(UART_BAUD*16UL)))-1)

#define SERIAL_BUFF_SIZE 50

unsigned char rxBuffer[SERIAL_BUFF_SIZE]={0X00};
uint8_t pos = 0;

// uart recieve interrupt service routine
ISR(USART_RXC_vect)
{
	if(pos < SERIAL_BUFF_SIZE) rxBuffer[ pos ++ ] = UDR;
}

// function for initialization of uart port
void uartBegin(long UART_BAUD, char parity, char stopBits)
{
	// 0X86h is the default value of UCSRC register
	unsigned char ucsrc = 0X86;

	//parity mode setting
	switch(parity)
	{
		case 'n':
		case 'N':
		ucsrc &= (~( (1<<UPM1) | (1<<UPM0) ) );
		break;

		case 'e':
		case 'E':
		ucsrc |= (1<<UPM1);
		ucsrc &= (~(1<<UPM0));
		break;

		case 'o':
		case 'O':
		ucsrc |= ( (1<<UPM1) | (1<<UPM0) );
		break;
	}


	//setting the number of stop bits
	if(stopBits==2)
		ucsrc |= (1<<USBS);
	else
		ucsrc &= (~(1<<USBS));

	//setting the data length
	ucsrc |= (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);


	UCSRB |= (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);

	//UCSRC = (1<<URSEL) | (1<<UCSZ0) | (1<<UCSZ1);
	UCSRC = ucsrc;

	//setting the baud rate
	UBRRL = BAUD_PRESCALER;
	UBRRH = (BAUD_PRESCALER >> 8 );

	sei();

}

// function to send a character through uart port
void uartSend(char ch)
{
	while( (UCSRA & (1<<UDRE)) == 0 );
	UDR = ch;
}

// function to read a character that was recieved through uart port
char uartRead(void)
{
	if( pos > 0 ) return rxBuffer[ --pos];
	else return -1;
}

// function to disable uart port
void uartDisable(void)
{
	UCSRB &= ( ~ ( (1<<RXEN) | (1<<TXEN) | (1<<RXCIE) ) );
}
