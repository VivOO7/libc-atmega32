#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD_PRESCALER (((F_CPU/(UART_BAUD*16UL)))-1)

#define SERIAL_BUFF_SIZE 50

#define STRING_SIZE 50

#define STOP_CHAR -1

static unsigned char rxBuffer[SERIAL_BUFF_SIZE]={0X00};
static uint8_t inPos = 0;
static uint8_t outPos = 0;
static uint8_t filled = 0;

char str[STRING_SIZE]={0X00};
uint8_t ind=0;

char strg[STRING_SIZE]={0X00};

static char empty_str[]="";

// uart receive interrupt service routine
ISR(USART_RXC_vect)
{
	if(filled < SERIAL_BUFF_SIZE)
	{
		rxBuffer[ inPos ++ ] = UDR;
		filled++;
		if(inPos == SERIAL_BUFF_SIZE) inPos=0;
	}
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

//function to send a string of character through uart port
void uartSendString(char *str)
{
	while( (*str) != '\0')
	{
		uartSend( *str );
		str+=1;
	}
}

// function to read a character that was received through uart port
char uartRead(void)
{
	if( filled > 0 )
	{
		char data=rxBuffer[outPos++];
		filled--;
		if(outPos==SERIAL_BUFF_SIZE) outPos=0;
		return data;
	}
	else return STOP_CHAR;
}

// function to disable uart port
void uartDisable(void)
{
	UCSRB &= ( ~ ( (1<<RXEN) | (1<<TXEN) | (1<<RXCIE) ) );
}

// function to read a string of passed length, received through the uart port
char * readString(uint16_t length)
{
	if(filled>=length)
	{
		for(unsigned int x=0;x<length;x++)
			str[x]=uartRead();
		str[length]='\0';
		return str;
	}
	else
	{
		return empty_str;
	}
}

// function to read a string delimited by the passed character, received through the uart port
char * readStringUntil(char delimiter)
{
	if(filled)
	{
		char ch=uartRead();
		if(ch==delimiter)
		{
			str[ind]='\0';
			ind=0;
			return str;
		}
		else
		{
			if(ind<STRING_SIZE-1)
				str[ind++]=ch;
			else
			{
				str[ind]='\0';
				ind=0;
				return str;
			}
			return empty_str;
		}
	}
	else
	{
		return empty_str;
	}
}

//function to print a character string to the uart port
void uartPrintln(char *st)
{
	uartSendString(st);
	uartSend('\n');
}
