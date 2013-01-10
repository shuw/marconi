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


#ifndef __SERIALINPUT_H__
	#define __SERIALINPUT_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
typedef struct
{
	WORD temp;
} SERIAL_INPUT_OBJ_STRUCT;


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void SerialInputInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD SerialInput(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
 WORD SerialInputReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);


#endif
