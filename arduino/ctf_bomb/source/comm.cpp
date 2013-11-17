/*************************************************************
project: <type project name here>
author: <type your name here>
description: <type what this file does>
*************************************************************/

#include "comm.h"

//-------------------------------------------------------------------

Comm comm (SERIAL_PORT);

//-------------------------------------------------------------------

Comm::Comm(int serialPort)
{
	serialId = serialPort; // Will be used in the future. Right now, it's useless.
	
	sequenceString = new char[MAX_SEQUENCE_SIZE * 2 + 1];
	//strcpy(sequenceString, "3F3E3C34");
	//strcpy(sequenceString, "3F3E3C34");
	strcpy(sequenceString, "-0-1-3");
}

//-------------------------------------------------------------------

Comm::~Comm()
{
	delete [] sequenceString;
}

//-------------------------------------------------------------------

void Comm::Initialize (void)
{
	Serial.begin(9600); 
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}  
	
	//Serial.println("mainframe-kaboom started!"); 
}

//-------------------------------------------------------------------

void Comm::SendMessage(char * const text)
{
	Serial.println(text);
}

//-------------------------------------------------------------------

char * Comm::GetSequenceString(void)
{
	return sequenceString;
}

//-------------------------------------------------------------------

unsigned char Comm::ReadCommands(void)
{
	unsigned char cmd = 0;		
	char header[3] = "";
	unsigned char count = 0;
	
	while (Serial.available() > 0) {
		unsigned char inByte = Serial.read();
		header[count] = inByte;
		++count;
		if (count == 3) {
			if ( (header[0] == '*') && (header[2] == '*')) {
				if (header[1] == 'A') {
					cmd = CMD_SET_SEQUENCE;
					strcpy(sequenceString, "");
					int l = strlen(sequenceString);
					while (Serial.available() > 0) {
						unsigned char inByte = Serial.read();						
						if (inByte == '|') {
							break;
						} else {
							sequenceString[l] = inByte;
							sequenceString[l+1] = 0;		
							++l;
							if (l >= MAX_SEQUENCE_SIZE * 2) {
								break;
							}
						}
						delay(10);
					}
					Serial.print("Sequence:[");
					Serial.print(sequenceString);
					Serial.print("]");
				} else if (header[1] == 'B') {					
					cmd = CMD_START;					
				} else if (header[1] == 'C') {
					cmd = CMD_GET_STATUS;
				}
				break;
			}
			count = 0;
		}
	}
	
	// Clean remaining characters at serial buffer, or get them, 
	// if command requires arguments	
	unsigned int pos = 0;
	while (Serial.available() > 0) {
		unsigned char inByte = Serial.read();
		if (cmd == CMD_SET_SEQUENCE) {
			if (pos < MAX_SEQUENCE_SIZE * 2 + 1) {
				sequenceString[pos] = inByte;
				++pos;
			}		
		}		
	}  
	
	return cmd;
}

//-------------------------------------------------------------------


