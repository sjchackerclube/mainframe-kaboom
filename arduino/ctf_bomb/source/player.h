//-------------------------------------------------------------------
#ifndef __player_H__
#define __player_H__
//-------------------------------------------------------------------

#include <arduino.h>
#include <kaboom_definitions.h>
#include <LiquidCrystal.h>
#include <game_clock.h>
#include <config.h>
#include <comm.h>

//-------------------------------------------------------------------

class Player {
public:
	Player (unsigned char playerId, unsigned char lcdEnable, 
			unsigned char wire0, unsigned char wire1, unsigned char wire2, 
			unsigned char wire3, unsigned char wire4, unsigned char wire5);
	~Player ();
	bool IsReady(void);
	void NotifyNotReady(void);
	bool Start (void); //(long gameTime, unsigned char * sequence, unsigned int sequenceSize);
	void Update(void);
	
	unsigned char GetWiringStatus (void);
	unsigned char GetStatus (void);
	unsigned char GetStep (void);
	
private:
	long time;
	long lastPrintedTime;
	char status;
	unsigned char id;
	unsigned char msg;
	unsigned char displayEnable;
	LiquidCrystal * lcd;
	unsigned char wirePins[6];
	unsigned char wiringStatus;
	unsigned int step;
	
	unsigned char ReadWiring (void);
	int GetRemovedWire(unsigned char previous, unsigned char current);
	bool WaitGetReady(void);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
