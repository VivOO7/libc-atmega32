#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "definitions.h"
#include "spi.h"

#define TRANSMIT_DELAY_REQUIRED

#define SLAVE_REQUIRED

#define SPI_BUFF_SIZE 25

#define SPI_STRING_SIZE 25

#define SPI_MASTER_MODE_READ_CHAR 0XFF

#define STOP_CHAR 0XFF

#define TRANSMIT_DELAY_US 1

#define SLAVE_NULL_CHAR 0x00

// this variable will store the mode of SPI port of the device
DEVICE_MODE _spiMode=master;

static unsigned char _STR[SPI_STRING_SIZE]={0X00};
static unsigned char emptyString[]="";
static uint8_t ind=0;

// receive and transmit buffers for SPI port in SLAVE mode
#ifdef SLAVE_REQUIRED

static unsigned char spiBufferRX[SPI_BUFF_SIZE]={0X00};
static uint8_t inPosRX = 0;
static uint8_t outPosRX = 0;
static uint8_t filledRX = 0;

static unsigned char spiBufferTX[SPI_BUFF_SIZE]={0X00};
static uint8_t inPosTX = 0;
static uint8_t outPosTX = 0;
static uint8_t filledTX = 0;

static unsigned char _CH=0;

static volatile unsigned char _LAST_CH = SLAVE_NULL_CHAR;

ISR(SPI_STC_vect)
{
	if(_spiMode == slave)
	{
		_CH=SPDR;

		if(filledTX > 0 && _CH == SPI_MASTER_MODE_READ_CHAR )
		{
			_LAST_CH=spiBufferTX[outPosTX++];
			SPDR = _LAST_CH;
			filledTX--;
			if(outPosTX == SPI_BUFF_SIZE) outPosTX=0;
		}
		else
		{
			if(_LAST_CH != SLAVE_NULL_CHAR)
			{
				if(outPosTX==0)
				{
					outPosTX=SPI_BUFF_SIZE-1;
					spiBufferTX[outPosTX]=_LAST_CH;
				}
				else
					spiBufferTX[--outPosTX]=_LAST_CH;
				filledTX++;
				_LAST_CH = SLAVE_NULL_CHAR;

			}
			SPDR = SLAVE_NULL_CHAR;
		}


		if(filledRX < SPI_BUFF_SIZE && _CH != SPI_MASTER_MODE_READ_CHAR)
		{
			spiBufferRX[ inPosRX ++ ] = _CH;
			filledRX++;
			if(inPosRX == SPI_BUFF_SIZE) inPosRX=0;
		}
	}
}

static uint8_t lengthOf(unsigned char * STR)
{
	long length=0;
	while( (*STR) != '\0' )
	{
		length+=1;
		STR+=1;
	}
	return length;
}

#endif


/*
 * DESCRIPTION - This function initializes the SPI port on the device. If the device needs to be initialized as a
 * master device then, the SS pin will be set as output by the function. Calling function will have no control
 * over this.
 *
 *PARAMETERS -
 *1. _DEV_MODE : this variable stores the device mode - master or slave
 *2. _DD : This variable stores the direction of the transmission - lsb_first or msb_first
 *3. _CLK_POL : This variable stores the polarity of the system - high_idle or low_idle
 *4. _STSP : This variable stores the SAMPLING edge of the SPI port - leading edge or trailing edge
 */
void spiBegin(DEVICE_MODE _DEV_MODE, DATA_DIRECTION _DD, CLOCK_POLARITY _CLK_POL, PULSE_EDGE _STSP)
{
	_spiMode=_DEV_MODE;
	unsigned char init_var=0;

	if(_DEV_MODE == master)
	{
		init_var |= (1<<MSTR) | (1<<SPR0);
		DDRB |= (1<<SS)|(1<<MOSI)|(1<<SCK);
	}
	else
	{
		init_var |= (1<<SPIE);
		DDRB |= (1<<MISO);
		SPDR = SLAVE_NULL_CHAR;
		sei();
	}

	if(_DD == lsb_first)
		init_var |= (1<<DORD);

	if(_CLK_POL == high_idle)
		init_var |= (1<<CPOL);

	if(_STSP == trailing_edge)
		init_var |= (1<<CPHA);

	// default clock frequency for the SPI port is fosc/16, this can be changed any time.
	init_var |= (1<<SPE);

	SPCR = init_var;
}

/*
 * MASTER MODE ROUTINES.
 */

unsigned char spiTransmit_Master(unsigned char sData)
{
	SPDR=sData;
	while( ! (SPSR & (1<<SPIF) ));
	return (SPDR);
}

void mspiSend(unsigned char sData)
{
	spiTransmit_Master(sData);
}

unsigned char mspiRead(void)
{
	return spiTransmit_Master(SPI_MASTER_MODE_READ_CHAR);
}

void mspiSendString(unsigned char * STR)
{
	while( (*STR) != '\0')
	{
		spiTransmit_Master( (*STR) );
		STR+=1;

#ifdef TRANSMIT_DELAY_REQUIRED
		_delay_us(TRANSMIT_DELAY_US);
#endif

	}
}

void mspiPrintln(unsigned char * STR)
{
	mspiSendString(STR);
	spiTransmit_Master('\n');
}

unsigned char * mspiReadStringUntil(unsigned char delimiter)
{
#ifdef TRANSMIT_DELAY_REQUIRED
	_delay_us(TRANSMIT_DELAY_US);
#endif

	char ch=spiTransmit_Master(SPI_MASTER_MODE_READ_CHAR);

	// enable advance filtering over here

	if(ch==SLAVE_NULL_CHAR) return emptyString;

	if( ind < SPI_STRING_SIZE-1 && ch != delimiter )
	{
		_STR[ind++]=ch;
		return emptyString;
	}
	else
	{
		_STR[ind]='\0';
		ind=0;
		return _STR;
	}
}

unsigned char * mspiReadString(uint8_t num_char)
{
	if(num_char >= SPI_STRING_SIZE)
		return emptyString;

#ifdef TRANSMIT_DELAY_REQUIRED
	_delay_us(TRANSMIT_DELAY_US);
#endif

	char ch=spiTransmit_Master(SPI_MASTER_MODE_READ_CHAR);

	if(ch==SLAVE_NULL_CHAR) return emptyString;

	//enable advance filtering over here

//	if( num_char > SPI_BUFF_SIZE-1)
//		return emptyString;

	if( ind < num_char )
	{
		_STR[ind++]=ch;
		return emptyString;
	}
	else
	{
		_STR[ind]='\0';
		ind=0;
		return _STR;
	}
}



/*
 * 	SLAVE MODE ROUTINES
 */

#ifdef SLAVE_REQUIRED

unsigned char sspiRead(void)
{
	//reading from the SPI receive buffer
	if( filledRX > 0 )
	{
		unsigned char data=spiBufferRX[outPosRX++];
		filledRX--;
		if(outPosRX == SPI_BUFF_SIZE) outPosRX=0;
		return data;
	}
	else return STOP_CHAR;

}

void sspiSend(unsigned char ch)
{
	//writing to the SPI transmit buffer
	if(filledTX < SPI_BUFF_SIZE)
	{
		spiBufferTX[ inPosTX++ ]=ch;
		filledTX++;
		if(inPosTX == SPI_BUFF_SIZE) inPosTX=0;
	}
}

unsigned char * sspiReadString(uint8_t length)
{
	/* Take a note of the below condition.
	 *
	 */
	if(filledRX>=length && length < SPI_STRING_SIZE)
	{
		for(unsigned int x=0;x<length;x++)
			_STR[x]=sspiRead();
		_STR[length]='\0';
		return _STR;
	}
	else
	{
		return emptyString;
	}
}

unsigned char * sspiReadStringUntil(unsigned char delimiter)
{
	if(filledRX)
	{
		char ch=sspiRead();

		if(ch==delimiter)
		{
			_STR[ind]='\0';
			ind=0;
			return _STR;
		}
		else
		{
			if(ind< SPI_STRING_SIZE-1)
				_STR[ind++]=ch;
			else
			{
				_STR[ind]='\0';
				ind=0;
				return _STR;
			}
			return emptyString;
		}
	}
	else
	{
		return emptyString;
	}

}


char sspiSendString(unsigned char * STR)
{
	uint8_t _l=lengthOf(STR);

	if( (SPI_BUFF_SIZE - filledTX) >= _l)
	{
		for(uint8_t x=0;x<_l;x++)
		{
			sspiSend(*(STR+x));
		}
		return 1;
	}
	else
		return 0;
}


char sspiPrintln(unsigned char * STR)
{
	uint8_t _l=lengthOf(STR);
	_l++;

	if( (SPI_BUFF_SIZE - filledTX) >= _l)
	{
		for(uint8_t x=0;x<_l;x++)
		{
			sspiSend(*(STR+x));
		}
		sspiSend('\n');
		return 1;
	}
	else
		return 0;
}


uint8_t sspiBufferStateTX(void)
{
	return filledTX;
}

uint8_t sspiBufferStateRX(void)
{
	return filledRX;
}

#endif

void selectDefaultSlave(void)
{
	PORTB &= (~(1<<SS));
}

void deselectDefaultSlave(void)
{
	PORTB |= (1<<SS);
}

