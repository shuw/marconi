//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		Enter Description here
//
//
//
//******************************************************************************


#ifndef __APPCHAT_H__
	#define __APPCHAT_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define SIZE_OF_CHAT_MSG_STACK  		50

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef enum
{
	WAITING_FOR_NEXT_MSG,
	SEND_MSG_HEADER,
	SEND_MSG_ENDER,
	SENDING_MSG,
	SEND_MSG_PAD,
	NUM_OF_CHAT_STATES
} CHAT_STATES;

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
typedef struct
{
	WORD state;
	WORD copyStartIndex;
	WORD msgPadSize;
} CHAT_OBJ_STRUCT;



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
WORD AppChat(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD AppChatReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);
void AppChatInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);


#endif
