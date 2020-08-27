#ifndef SPI_H_
#define SPI_H_

#define SLAVE_REQUIRED

#include "definitions.h"

#define MOSI 5
#define MISO 6
#define SCK 7
#define SS 4

void spiBegin(DEVICE_MODE, DATA_DIRECTION, CLOCK_POLARITY, PULSE_EDGE);

unsigned char spiTransmit_Master(unsigned char );

void mspiSend(unsigned char );

unsigned char mspiRead(void );

void mspiSendString(unsigned char *);

void mspiPrintln(unsigned char *);

unsigned char * mspiReadStringUntil(unsigned char );

unsigned char * mspiReadString(uint8_t );

#ifdef SLAVE_REQUIRED

unsigned char sspiRead(void );

void sspiSend(unsigned char );

unsigned char * sspiReadStringUntil(unsigned char );

unsigned char * sspiReadString(uint8_t );

char sspiSendString(unsigned char *);

char sspiPrintln(unsigned char *);

uint8_t sspiBufferStateTX(void);

uint8_t sspiBufferStateRX(void);

#endif

void selectDefaultSlave(void);

void deselectDefaultSlave(void);

#endif
