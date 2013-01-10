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


#ifndef __AWGN_H__
	#define __AWGN_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define INV_AVERAGE_PERIOD_OF_BIT_DROPS			0x0

#define LRAND_MAX 0x7FFFFFFF
#define AWGN_NUM_OF_BITS_PER_SYMBOL					16
#define AWGN_SYMBOL_NORMALIZATION					0x7FFF

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef struct
{
	double SignalToNoiseDB;
} AWGN_CHANNEL_INIT_STRUCT;

typedef struct
{
	double SignalToNoiseDB;
	BYTE currentState;
	WORD numOfDroppedBits;
} AWGN_CHANNEL_OBJ_STRUCT;

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define AWGN_READY_SIZE			8


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void AWGNChannelInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD AWGNChannel(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD AWGNChannelReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);

#endif
