#include <avr/io.h>

#include "String.h"
#include "AT.h"

#define AT_ADVANCED_CHECK_ENABLED

#define BREAK_CHAR '='

#define AT_COMMAND_MAX_LENGTH 10
#define AT_PARAM_MAX_LENGTH 10

// the character array to store the AT commands and AT Parameters in
static char atCommand[AT_COMMAND_MAX_LENGTH+1]={0};
static char atParam1[AT_PARAM_MAX_LENGTH+1]={0};
static char atParam2[AT_PARAM_MAX_LENGTH+1]={0};
static char atParam3[AT_PARAM_MAX_LENGTH+1]={0};
static char atParam4[AT_PARAM_MAX_LENGTH+1]={0};

char * _tempSTR;
char _TMPSTR[ 4 * AT_PARAM_MAX_LENGTH + 1];
// declaring an AT command structure
static AT_COMMAND command={ 0, 0, atCommand, {atParam1, atParam2, atParam3, atParam4}};


AT_COMMAND getCommand(char * _input_string)
{
	uint8_t _L;

	command.isAT=0;
	command._numParams=0;

	atCommand[0]='\0';
	atParam1[0]='\0';
	atParam2[0]='\0';
	atParam3[0]='\0';
	atParam4[0]='\0';


#ifdef AT_ADVANCED_CHECK_ENABLED

	if( searchString(_input_string,"AT") )
		return command;

	_L=lengthOf(_input_string);

	if(_L==2)
	{
		// attention command
		command.isAT=1;
		command._numParams=0;
		atCommand[0]='\0';
		return command;
	}
	else if(_L<5 || *(_input_string+2)!='+')
		return command;
#endif

	_L=find(_input_string,'=');

#ifdef AT_ADVANCED_CHECK_ENABLED
	if(_L<4)
		return command;
#endif

	command.isAT=1;

	//determining the command string and storing it in the atCommand[] string variable
	_tempSTR=substring(_input_string,3,_L);
	deepCopyString(_tempSTR,atCommand,AT_COMMAND_MAX_LENGTH);

	// now tempSTR contains the parameters separated by break character
	_tempSTR=substring(_input_string,_L+1,lengthOf(_input_string));
	if(equals(_tempSTR,"")) return command;

	for(uint8_t z=0;z<4;z++)
	{
		deepCopyString(_tempSTR,_TMPSTR, (4 * AT_PARAM_MAX_LENGTH) );

		_tempSTR=splitByCharacter(_TMPSTR, BREAK_CHAR);
		//copying the second parameter in the atParam2 string
		deepCopyString(_TMPSTR, command._params[z], AT_PARAM_MAX_LENGTH);
		command._numParams++;
		if(equals(_tempSTR,"")) return command;

	}

}

