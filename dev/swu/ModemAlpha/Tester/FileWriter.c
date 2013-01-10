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


#define __FILEWRITER_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
static FILE * fileOutputtestStream;


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
FILE_FORMAT_ENUM writeFileFormat;

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
void FileWriterInit(BYTE *outputFileNameString, FILE_FORMAT_ENUM format)
{
	fileOutputtestStream = fopen(outputFileNameString , "w+");
	writeFileFormat = format;
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
BYTE FileWriter(BYTE *inBuffer, BYTE *inSize, BYTE *outBuffer, BYTE *outSize)
{
	BYTE inputSize = *inSize;
	static BYTE counter = 0;
	BYTE bitCounter = 8;
	
	while ( inputSize-- > 0)
	{
		counter ++;

		switch (writeFileFormat)
		{
			case (HEX_FILE):
				fprintf(fileOutputtestStream, "%02X ", (WORD)(*inBuffer++ & 0x00FF));
		//		PrintMsgP("\nOut Buffer: %02X", (WORD)(*inBuffer & 0x00FF));

				if (counter == 16)
				{
					counter  = 0;
					fprintf(fileOutputtestStream, "\n");
				}
				break;
			case (BINARYSTRING_FILE):
				bitCounter = 8;
				
				while (bitCounter-- > 0)
				{
					fprintf(fileOutputtestStream, "%01d", (WORD)(*inBuffer & 0x0080) >> 7);
					*inBuffer = *inBuffer << 1;
				}
				fprintf(fileOutputtestStream, " ");
				inBuffer++;

				if (counter == 4)
				{
					counter  = 0;
					fprintf(fileOutputtestStream, "\n");
				}
				break;
			default:
				break;
		}
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
BYTE FileWriterInputSizeB(void)
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
BYTE FileWriterOutputSizeB(void)
{
	return 0;
}


/***********************************  END  ************************************/
