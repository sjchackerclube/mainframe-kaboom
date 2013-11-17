/*************************************************************
project: <type project name here>
author: <type your name here>
description: <type what this file does>
*************************************************************/

#include "player.h"

//-----------------------------------------------------------------------------

Player::Player (unsigned char playerId, unsigned char lcdEnable,
				unsigned char wire0, unsigned char wire1, unsigned char wire2, 
				unsigned char wire3, unsigned char wire4, unsigned char wire5)
{	
	id = playerId;
	displayEnable = lcdEnable;
	
	wirePins[0] = wire0;
	wirePins[1] = wire1;
	wirePins[2] = wire2;
	wirePins[3] = wire3;
	wirePins[4] = wire4;
	wirePins[5] = wire5;
	
	// Activate the internal pull-up resistor for all wiring pins
	for (unsigned int i=0; i < 6; i++) {
		pinMode(wirePins[i], INPUT); 
		digitalWrite(wirePins[i], HIGH);
	}

	wiringStatus = 0;
	
	status = PLAYER_STATUS_WAITING;	
	time = 0;
	step = 0;
	msg = MSG_NONE;
	
	lcd = new LiquidCrystal (LCD_RS, displayEnable,	
							LCD_DATA_0, LCD_DATA_1, LCD_DATA_2, LCD_DATA_3); 
	lcd->begin (16, 2);							
}

//-----------------------------------------------------------------------------

Player::~Player()
{
	delete lcd;
	lcd = NULL;	
}

//-----------------------------------------------------------------------------

bool Player::Start (void)
{
	if (status != PLAYER_STATUS_NOT_READY) {
		time = config.GetMaxtime();
		status = PLAYER_STATUS_PLAYING;
		msg = MSG_NONE;		
	}
	
	lcd->clear();		
	step = 0;
	lastPrintedTime = 0;
	wiringStatus = 63;
	
	Serial.print("time:");
	Serial.println(time);
	
	return IsReady();
	
}

//-----------------------------------------------------------------------------

bool Player::IsReady(void)
{
	// return true if all wires are connected, so the player can start the game
	return (ReadWiring() == 63);
}

//-----------------------------------------------------------------------------

void Player::NotifyNotReady(void)
{
	status = PLAYER_STATUS_NOT_READY;
	msg = MSG_NOT_READY;
}

//-----------------------------------------------------------------------------

bool Player::WaitGetReady(void)
{
	if (status == PLAYER_STATUS_NOT_READY) {
		//if (ReadWiring() == 0x3F) {
		if (IsReady()) {
			status = PLAYER_STATUS_WAITING;
			Start();
			
			return true;
		} else {
			lcd->setCursor(0, 0);
			lcd->print("Fio desconect.:       "); 
			lcd->setCursor(0, 1);
			lcd->print("                      "); 
			lcd->setCursor(0, 1);
			for (int i=0; i < 6; i++) {		
				unsigned char w = digitalRead(wirePins[i]);
				if (w != 0) {
					lcd->print(i);
				} else {
					lcd->print(" ");
				}
				lcd->print(" ");
			}		
			
			delay(500);
			
			return false;
		}
		
	} else {	
		return true;
	}
}

//-----------------------------------------------------------------------------

void Player::Update(void)
{	
	if (WaitGetReady() == false) {
		return;
	}
	
	bool refreshMsg = false;
	
	unsigned char lastWiringStatus = wiringStatus;			
	unsigned char lastMSg = msg;
	if (status == PLAYER_STATUS_PLAYING) {		
		// Check if player changed any wire		
		wiringStatus = ReadWiring();		
		if (wiringStatus != lastWiringStatus) {
			//Serial.print(">>");
			//Serial.print(wiringStatus);
			//Serial.print(",");
			//Serial.println(config.GetSequenceItem(step));
			unsigned char prevStatus = 63;
			if (step > 0) {
				prevStatus = config.GetSequenceItem(step-1);
			}
			if ((msg == MSG_OPS) && (wiringStatus == prevStatus)) {
				msg = MSG_CONTINUE;
			} else {
				if (config.GetSequenceItem(step) == wiringStatus) {
					++step;
					msg = MSG_CORRECT;
					refreshMsg = true;
					//Serial.print("::");
					//Serial.print(step);
					//Serial.print(",");
					//Serial.println(config.GetSequenceSize());
					if (step >= config.GetSequenceSize()) {
						status = PLAYER_STATUS_WON;					
						msg = MSG_WON;
					}
				} else {
					//time -= config.GetPenalty();
					msg = MSG_OPS;
				}			
			}
		}		
	}
	
	if (msg == MSG_OPS) {
		if (time > 0) {		
			--time;
		}
	}
	
	//time = 500;
	
	if ((clock.GetTimeSeconds() > time) && (status != PLAYER_STATUS_LOST)) {
		status = PLAYER_STATUS_LOST;
		msg = MSG_LOST;
	} 
	
	// if player message shall change, let's print it on 
	if (((lastMSg != msg) || (status == PLAYER_STATUS_NOT_READY)) || (refreshMsg)) {
		lcd->setCursor(0, 0);
		lcd->print("                ");			
		lcd->setCursor(0, 0);
		if (msg == MSG_GO_PLAYER) {
			lcd->print("Desarme a bomba!"); 
		} else if (msg == MSG_OPS) {
			lcd->print("Ops, fio errado!"); 
		} else if (msg == MSG_CORRECT) {
			int p = GetRemovedWire(lastWiringStatus, wiringStatus);			
			lcd->print("Ok: "); 
			if (p > 0) {
				lcd->print(p); 				
				lcd->print(" conec.");
			} else {
				lcd->print(-p); 
				lcd->print(" descon.");
			}
		} else if (msg == MSG_WON) {
			lcd->print("Voce venceu!"); 
		} else if (msg == MSG_LOST) {
			lcd->print("Perdeu, mano!"); 			
		} else if (msg == MSG_CONTINUE) {
			lcd->print("Continue..."); 			
		}						
	}	
	
	if (status == PLAYER_STATUS_NOT_READY) {
		if (ReadWiring() == 0x3F) {
			status = PLAYER_STATUS_WAITING;
			Start();
		} else {
			char text[17] = "";		
			for (int i=0; i < 6; i++) {		
				char w = digitalRead(wirePins[i]);
				if (w != 0) {
					Serial.print(i);
					strcat(text, &w);//sprintf(&(text[i*2]), "%d", i);
					strcat(text, " ");
				}
			}	
			Serial.println("");
			lcd->setCursor(0, 1);
			lcd->print(text);				
		}
	} else {	
		if (status != PLAYER_STATUS_WON) {
			if ( (millis() - lastPrintedTime > 800) || (msg == MSG_OPS) ) {
				long t = time - clock.GetTimeSeconds();
				lastPrintedTime = millis();
				if (t > 0) { 
					int h,m,s;
					h = t / 3600;
					t = t - (h * 3600);
					m = t / 60;
					s = t - (m * 60);
					char text[17];
					sprintf(text, "%02d:%02d:%02d", h, m, s);
					lcd->setCursor(0, 1);
					lcd->print(text);				
				} else {
					lcd->setCursor(0, 1);
					lcd->print("KABOOM!!!       ");				
				}
			} 		
			/*char text[17];
			sprintf(text, "%d:%d:%d:%d", time + 10, wiringStatus, config.GetSequenceItem(step), config.GetSequenceSize());
			lcd->setCursor(0, 1);
			lcd->print(text);				*/
		}
	}		
}

//-----------------------------------------------------------------------------

int Player::GetRemovedWire(unsigned char previous, unsigned char current)
{
	for (int i=0; i < 7; i++) {
		unsigned char bit1 = (previous & (1 << i));
		unsigned char bit2 = (current & (1 << i));
		
		
		if (bit1 != bit2) {
			if (bit2) {				
				return i;
			} else {
				return -i;
			}			
		}
	}
	
	return 0;
	
}

//-----------------------------------------------------------------------------

unsigned char Player::ReadWiring (void)
{
	unsigned char wires = 0;
	
	for (unsigned int i=0; i < 6; i++) {
		unsigned char w = digitalRead(wirePins[i]);
		if (w == 0) { // 0 = wire on ground = wire not removed
			wires = wires | (1 << i);
		}		
	}
	
	return wires;
}

//-----------------------------------------------------------------------------

unsigned char Player::GetWiringStatus (void)
{
	return ReadWiring(); //wiringStatus;	
}

//-----------------------------------------------------------------------------

unsigned char Player::GetStatus (void)
{
	return status;	
}

//-----------------------------------------------------------------------------

unsigned char Player::GetStep (void)
{
	return step;
}

//-----------------------------------------------------------------------------