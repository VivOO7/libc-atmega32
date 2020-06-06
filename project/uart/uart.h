#ifndef UART
#define UART

// function for initialization of uart port
void uartBegin(long,char,char);

// function to send a character through uart port
void uartSend(char);

// function to read a character that was recieved through uart port
char uartRead(void);

// function to disable uart port
void uartDisable(void);

#endif
