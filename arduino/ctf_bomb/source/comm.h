//-------------------------------------------------------------------
#ifndef __comm_H__
#define __comm_H__
//-------------------------------------------------------------------

#include <arduino.h>
#include <kaboom_definitions.h>

//-------------------------------------------------------------------

class Comm {
public:
	Comm(int serialPort);
	~Comm(void);
	void Initialize (void);
	void SendMessage(char * const text);
	unsigned char ReadCommands(void);
	char * GetSequenceString(void);
	
private:
	int serialId;
	char * sequenceString;
	//Serial 
};

//-------------------------------------------------------------------

extern Comm comm;

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
