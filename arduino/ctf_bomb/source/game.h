//-------------------------------------------------------------------
#ifndef __game_H__
#define __game_H__
//-------------------------------------------------------------------

#include <arduino.h>
#include <kaboom_definitions.h>
#include <game_clock.h>
#include <player.h>
#include <config.h>
#include <comm.h>

//-------------------------------------------------------------------

class Game {
public:
	Game(void);
	~Game();	
	bool Start(void);
	void Update(void);
	void SendStatusString(void);
	
private:
	Player * player[6];
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
