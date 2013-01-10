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


#ifndef __VDECODER_H__
	#define __VDECODER_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//#define VITERBI_MAX_PATH_LENGTH (VITERBI_CONSTRAINT_LENGTH * 4)

//for testing
#define VITERBI_MAX_PATH_LENGTH 		VENCODER_PACKET_SIZE * NUM_OF_BITS_IN_BBYTE

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void VDecoderInit(void);
BYTE VDecoder(BYTE *inBuffer, BYTE *inSize, BYTE *outBuffer, BYTE *outSize);
BYTE VDecoderInputSizeB(void);
BYTE VDecoderOutputSizeB(void);

#endif
