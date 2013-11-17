//-------------------------------------------------------------------
#ifndef __kaboom_definitions_H__
#define __kaboom_definitions_H__
//-------------------------------------------------------------------

#include <arduino.h>

//-------------------------------------------------------------------

#define MAX_SEQUENCE_SIZE		10
#define SERIAL_PORT				1

#define PLAYER_0_WIRE_0			53
#define PLAYER_0_WIRE_1			52
#define PLAYER_0_WIRE_2			51
#define PLAYER_0_WIRE_3			50
#define PLAYER_0_WIRE_4			49
#define PLAYER_0_WIRE_5			48

#define PLAYER_1_WIRE_0			19
#define PLAYER_1_WIRE_1			19
#define PLAYER_1_WIRE_2			19
#define PLAYER_1_WIRE_3			19
#define PLAYER_1_WIRE_4			19
#define PLAYER_1_WIRE_5			19

#define PLAYER_2_WIRE_0			19
#define PLAYER_2_WIRE_1			19
#define PLAYER_2_WIRE_2			19
#define PLAYER_2_WIRE_3			19
#define PLAYER_2_WIRE_4			19
#define PLAYER_2_WIRE_5			19

#define PLAYER_3_WIRE_0			19
#define PLAYER_3_WIRE_1			19
#define PLAYER_3_WIRE_2			19
#define PLAYER_3_WIRE_3			19
#define PLAYER_3_WIRE_4			19
#define PLAYER_3_WIRE_5			19

#define PLAYER_4_WIRE_0			19
#define PLAYER_4_WIRE_1			19
#define PLAYER_4_WIRE_2			19
#define PLAYER_4_WIRE_3			19
#define PLAYER_4_WIRE_4			19
#define PLAYER_4_WIRE_5			19

#define PLAYER_5_WIRE_0			19
#define PLAYER_5_WIRE_1			19
#define PLAYER_5_WIRE_2			19
#define PLAYER_5_WIRE_3			19
#define PLAYER_5_WIRE_4			19
#define PLAYER_5_WIRE_5			19

#define PLAYER_0_ENABLE_LCD		8
#define PLAYER_1_ENABLE_LCD		7
#define PLAYER_2_ENABLE_LCD		6
#define PLAYER_3_ENABLE_LCD		5
#define PLAYER_4_ENABLE_LCD		4
#define PLAYER_5_ENABLE_LCD		3

#define LCD_RS					9
//#define LCD_RW				10
#define LCD_DATA_0				10
#define LCD_DATA_1				11
#define LCD_DATA_2				12
#define LCD_DATA_3				13


#define PLAYER_STATUS_WAITING	0
#define PLAYER_STATUS_PLAYING	1
#define PLAYER_STATUS_LOST		2
#define PLAYER_STATUS_WON		3
#define PLAYER_STATUS_NOT_READY	4

#define MSG_NONE				0
#define MSG_GO_PLAYER			1
#define MSG_OPS					2
#define MSG_CORRECT				3
#define MSG_WON					4
#define MSG_LOST				5
#define	MSG_NOT_READY			6
#define MSG_CONTINUE			7

#define ERROR_NONE				0

#define CMD_NONE				0
#define CMD_SET_SEQUENCE		1
#define CMD_START				2
#define CMD_GET_STATUS			3



//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
