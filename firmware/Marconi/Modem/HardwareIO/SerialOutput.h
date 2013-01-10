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


#ifndef __SERIALOUTPUT_H__
	#define __SERIALOUTPUT_H__


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
	WORD outputType;
} SERIAL_OUTPUT_OBJ_STRUCT;


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void SerialOutputInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD SerialOutput(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
 WORD SerialOutputReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);


#endif
