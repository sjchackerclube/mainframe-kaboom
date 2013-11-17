/*************************************************************
project: <type project name here>
author: <type your name here>
description: <type what this file does>
*************************************************************/

#include "game.h"

//-------------------------------------------------------------------

Game::Game(void)
{
	player[0] = new Player (0, PLAYER_0_ENABLE_LCD, PLAYER_0_WIRE_0, 
							PLAYER_0_WIRE_1, PLAYER_0_WIRE_2, 
							PLAYER_0_WIRE_3, PLAYER_0_WIRE_4, 
							PLAYER_0_WIRE_5);
							
	player[1] = new Player (1, PLAYER_1_ENABLE_LCD, PLAYER_1_WIRE_0, 
							PLAYER_1_WIRE_1, PLAYER_1_WIRE_2, 
							PLAYER_1_WIRE_3, PLAYER_1_WIRE_4, 
							PLAYER_1_WIRE_5);						
							
	player[2] = new Player (2, PLAYER_2_ENABLE_LCD, PLAYER_2_WIRE_0, 
							PLAYER_2_WIRE_1, PLAYER_2_WIRE_2, 
							PLAYER_2_WIRE_3, PLAYER_2_WIRE_4, 
							PLAYER_2_WIRE_5);						

	player[3] = new Player (3, PLAYER_3_ENABLE_LCD, PLAYER_3_WIRE_0, 
							PLAYER_3_WIRE_1, PLAYER_3_WIRE_2, 
							PLAYER_3_WIRE_3, PLAYER_3_WIRE_4, 
							PLAYER_3_WIRE_5);						

	player[4] = new Player (4, PLAYER_4_ENABLE_LCD, PLAYER_4_WIRE_0, 
							PLAYER_4_WIRE_1, PLAYER_4_WIRE_2, 
							PLAYER_4_WIRE_3, PLAYER_4_WIRE_4, 
							PLAYER_4_WIRE_5);						

	player[5] = new Player (5, PLAYER_5_ENABLE_LCD, PLAYER_5_WIRE_0, 
							PLAYER_5_WIRE_1, PLAYER_5_WIRE_2, 
							PLAYER_5_WIRE_3, PLAYER_5_WIRE_4, 
							PLAYER_5_WIRE_5);													
}

//-------------------------------------------------------------------

Game::~Game()
{
	for (unsigned int i=0; i < 6; i++) {
		delete player[i];
		player[i] = NULL;
	}
}
	
//-------------------------------------------------------------------	

bool Game::Start(void)
{
	for (unsigned int i=0; i < 6; i++) {
		if (player[i]->IsReady() == false) {
			//char msg[30];
			//sprintf(msg, "+Player not ready:%d+", i);
			//comm.SendMessage(msg);
			player[i]->NotifyNotReady();
			//return false;
		}
	}
	
	config.SetSequence(comm.GetSequenceString());
	
	clock.Start();
	
	for (unsigned int i=0; i < 6; i++) {
		player[i]->Start();
	}
	
	return true;
}
	
//-------------------------------------------------------------------	

void Game::Update(void)
{
	/*for (unsigned int i=0; i < 6; i++) {
		player[i]->Update();
	}*/
	player[0]->Update();
}
	
//-------------------------------------------------------------------	

void Game::SendStatusString(void)
{
	char buffer[70] = "*";
	
	for (unsigned int i=0; i < 6; i++) {		
		unsigned char status = player[i]->GetStatus();	
		unsigned char step   = player[i]->GetStep ();
		unsigned char wire   = player[i]->GetWiringStatus ();		
		sprintf(&(buffer[i * 9 + 1]), "%02X:%02X:%02X|", step, status, wire);
	} 
	
	comm.SendMessage(buffer);
}
	
//-------------------------------------------------------------------	