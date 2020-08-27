#ifndef UART
#define UART

// function for initialization of uart port
void uartBegin(long,char,char);

// function to send a character through uart port
void uartSend(char);

//function to sends a string of character through the uart port
void uartSendString(char *);

// function to read a character that was received through uart port
char uartRead(void);

// function to disable uart port
void uartDisable(void);

// function to read a string of specified length from serial port
char * readString(uint16_t);

// function to read a string delimited by the passed character
char * readStringUntil(char );

void uartPrintln(char *);

#endif
