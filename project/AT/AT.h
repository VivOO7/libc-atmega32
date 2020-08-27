#ifndef AT_H_
#define AT_H_

typedef struct AT_COMMAND
{
	char isAT;
	uint8_t _numParams;
	char * _command;
	char * _params[4];
}AT_COMMAND;


AT_COMMAND getCommand(char * );

#endif
