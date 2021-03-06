//-------------------------------------------------------------------
#ifndef __game_clock_H__
#define __game_clock_H__
//-------------------------------------------------------------------

#include <arduino.h>

//-------------------------------------------------------------------

class Clock {
public:
	void Start(void);
	unsigned int GetTimeSeconds(void);
	unsigned int GetTimeMiliseconds(void);
private:
	unsigned int time;
};


//-------------------------------------------------------------------

extern Clock clock;

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
