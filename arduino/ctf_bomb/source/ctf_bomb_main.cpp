#include "ctf_bomb_main.h"

//-----------------------------------------------------------------------------

Game game;

//-----------------------------------------------------------------------------

void setup() {
	comm.Initialize();	
	//pinMode(13, OUTPUT);    
	game.Start(); 
}

//-----------------------------------------------------------------------------

void loop() {
	unsigned char cmd = comm.ReadCommands();
	
	if (cmd == CMD_GET_STATUS) {
		game.SendStatusString();
	} else if (cmd == CMD_START) {	
		game.Start();
	} 
	
	game.Update();
	
	delay(50);  
	
	
	
}

//-----------------------------------------------------------------------------
