//-------------------------------------------------------------------
#ifndef __config_H__
#define __config_H__
//-------------------------------------------------------------------

#include <arduino.h>
#include <kaboom_definitions.h>

//-------------------------------------------------------------------

class Config {
public:
	Config(void);
	~Config();
	
	void SetMaxTime (int maxtime);
	int GetMaxtime (void);
	
	void SetPenalty (int timePenalty);
	int GetPenalty (void);
	
	void SetSequence (char * const text);
	void SetSequence (unsigned char * sequence, unsigned int sequenceSize);
	unsigned int GetSequenceSize (void);
	unsigned char GetSequenceItem (unsigned int index);
	
	unsigned char Hex2Dec(char *hex);
	
private:
	unsigned int penalty;
	unsigned int maxTime;
	unsigned char * seq; 
	unsigned int seqSize;
};


//-------------------------------------------------------------------

extern Config config;

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
