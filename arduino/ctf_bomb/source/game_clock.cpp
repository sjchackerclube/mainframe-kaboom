/*************************************************************
project: <type project name here>
author: <type your name here>
description: <type what this file does>
*************************************************************/

#include "game_clock.h"

//-------------------------------------------------------------------

Clock clock;    

//-------------------------------------------------------------------

void Clock::Start(void)
{
	time = millis();
	
	Serial.print("Resetando tempo:");
	Serial.println(GetTimeSeconds());
}

//-------------------------------------------------------------------

unsigned int Clock::GetTimeSeconds(void)
{
	return (millis() - time) / 1000;
}

//-------------------------------------------------------------------

unsigned int Clock::GetTimeMiliseconds(void)
{
	return (millis() - time);
}

//-------------------------------------------------------------------

