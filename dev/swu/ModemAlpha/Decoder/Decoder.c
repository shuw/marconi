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


#define __DECODER_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  C O D E
//******************************************************************************


//******************************************************************************
//
// FUNCTION:        N/A
//
// USAGE:             N/A
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
BYTE Decoder(BYTE *inBuffer, BYTE *inSize, BYTE *outBuffer, BYTE *outSize)
{
	BYTE byteCounter;

	if (*outSize != (*inSize / 2))
	{
		PrintMsg("\nThis is a test decoder, output packet size must be 2 * input packet size\n");
	}
	for (byteCounter = 0; byteCounter < *outSize; byteCounter++)
	{
		if (*inBuffer != *(inBuffer +1))
		{
			*outBuffer++ = (BYTE)0xFF;
		}
		else
		{
			*outBuffer++ = *inBuffer;
		}
		inBuffer += 2;
	}

	return 0;
}


//******************************************************************************
//
// FUNCTION:        N/A
//
// USAGE:             N/A
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
BYTE DecoderInputSizeB(void)
{
	return 4;
}


//******************************************************************************
//
// FUNCTION:        N/A
//
// USAGE:             N/A
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
BYTE DecoderOutputSizeB(void)
{
	return 2;
}


/***********************************  END  ************************************/