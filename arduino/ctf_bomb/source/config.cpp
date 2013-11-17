/*************************************************************
project: <type project name here>
author: <type your name here>
description: <type what this file does>
*************************************************************/

#include "config.h"

//-------------------------------------------------------------------

Config config;

//-------------------------------------------------------------------

Config::Config(void)
{
	penalty = 30;
	maxTime = 5 * 60; // 5 minutes
	seq = NULL;
	seqSize = 0;
}

//-------------------------------------------------------------------

Config::~Config()
{
	if (seq != NULL) {
		delete []seq;
		seq = NULL;
		seqSize = 0;
	}
}

//-------------------------------------------------------------------

void Config::SetMaxTime (int maxtime)
{
	maxTime = maxtime;
}

//-------------------------------------------------------------------

int Config::GetMaxtime (void)
{
	return maxTime;
}
	
//-------------------------------------------------------------------	
	
void Config::SetPenalty (int timePenalty)
{
	penalty = timePenalty;
}

//-------------------------------------------------------------------	

int Config::GetPenalty (void)
{
	return penalty;
}
	
//-------------------------------------------------------------------		

unsigned char Config::Hex2Dec(char *hex) 
{
	unsigned char ret = 0; 
   
	if ((hex[0] > 47) && (hex[0] < 58)) {
		ret = 16 * (hex[0] - 48);
	} else if ((hex[0] > 64) && (hex[0] < 71)) {
		ret = 16 * (hex[0] - 55);
	} else if ((hex[0] > 96) && (hex[0] < 103)) {
		ret = 16 * (hex[0] - 87);
	}
	
	if ((hex[1] > 47) && (hex[1] < 58)) {
		ret += (hex[1] - 48);
	} else if ((hex[1] > 64) && (hex[1] < 71)) {
		ret += (hex[1] - 55);
	} else if ((hex[1] > 96) && (hex[1] < 103)) {
		ret += (hex[1] - 87);
	}
	
   return ret; 
}

//-------------------------------------------------------------------		

/*void Config::SetSequence (char * const text)
{
	unsigned int pos = 0;
	
	if (seq != NULL) {
		delete [] seq;
		seq = NULL;
		seqSize = 0;
	}	
	
	seqSize = strlen(text) / 2;
	if (seqSize > MAX_SEQUENCE_SIZE) {
		seqSize = MAX_SEQUENCE_SIZE;
	}
	
	seq = new unsigned char[seqSize];	
	memset(seq, 0, seqSize);
	
	// small-memory-footprint algorithm to convert the string sequence into a numeric sequence
	while (pos < strlen(text)) {	
		char temp[3] = "";//0x";		
		memcpy(&(temp[0]), &(text[pos]), 2);
		temp[2] = 0;
		if ( (pos / 2) < seqSize) {
			seq[pos / 2] = Hex2Dec(temp);//strtol(temp, NULL, 10); //atoi(temp);				
		}
		
		
		pos += 2;		
	}	
}*/

void Config::SetSequence (char * const text)
{
	unsigned int pos = 0;
	
	if (seq != NULL) {
		delete [] seq;
		seq = NULL;
		seqSize = 0;
	}	
	
	seqSize = strlen(text) / 2;
	if (seqSize > MAX_SEQUENCE_SIZE) {
		seqSize = MAX_SEQUENCE_SIZE;
	}
	
	seq = new unsigned char[seqSize];	
	memset(seq, 0, seqSize);
	unsigned char value = 0x3F;
	//seq[0] = value; // First position is always all wires connected	
		
	// small-memory-footprint algorithm to convert the string sequence into a numeric sequence
	while (pos < strlen(text)) {	
		if ( (pos / 2) >= seqSize) {
			break;
		}
		
		///unsigned char lastSeq = seq[0];
		unsigned char bit = text[pos + 1] - '0';
		
		if (text[pos] == '+') {
			seq[pos / 2] = value | (1 << bit);
		} else {
			seq[pos / 2] = value & (~(1 << bit));
		}	
		value = seq[pos / 2];
		
		pos += 2;		
	}	
}

//-------------------------------------------------------------------		

void Config::SetSequence (unsigned char * sequence, unsigned int sequenceSize)
{
	if (sequenceSize < MAX_SEQUENCE_SIZE) {
		seqSize = sequenceSize;
	} else {
		seqSize = MAX_SEQUENCE_SIZE;
	}
	
	if (seq != NULL) {
		delete [] seq;
		seq = NULL;
		seqSize = 0;
	}	
	
	seqSize = sequenceSize;
	seq = new unsigned char[seqSize];	
	
	memcpy(seq, sequence, seqSize);	
}

//-------------------------------------------------------------------

unsigned int Config::GetSequenceSize (void)
{
	return seqSize;
}
	
//-------------------------------------------------------------------
	
unsigned char Config::GetSequenceItem (unsigned int index)
{
	if (index < seqSize) {
		return seq[index];
	} else {
		return 0xFF;
	}
}

//-------------------------------------------------------------------

	
