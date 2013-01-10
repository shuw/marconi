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


#define __FILEREADER_C__


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
BOOL ParseByteHexValueFromFile(FILE *fileStream, BYTE *byteReturned);
BOOL ParseBinaryStringValueFromFile(FILE *fileStream, BYTE *byteReturned);
	
//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
static FILE * fileInputTestStream;
static BOOL fileReaderFinished = FALSE;

FILE_FORMAT_ENUM readFileFormat;

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
void FileReaderInit(BYTE *inputFileNameSring, FILE_FORMAT_ENUM format)
{
	// open a file for reading
	fileInputTestStream = fopen(inputFileNameSring , "r");
	readFileFormat = format;
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
// TODO: Make a generic tester based around controller

BYTE FileReader(BYTE *inBuffer, BYTE *inSize, BYTE *outBuffer, BYTE *outSize)
{
	BYTE byteParsed = 0;
	BYTE outSizeB = *outSize;
	//
	// Encoder Test Code
	//
	while ( !fileReaderFinished && outSizeB-- > 0)
	{
		switch (readFileFormat)
		{
			case (HEX_FILE):
				
				if ( ParseByteHexValueFromFile(fileInputTestStream, &byteParsed) == TRUE )
				{
					*outBuffer++ = byteParsed;

		//			PrintMsgP("\nIn Buffer %02X", (WORD)(byteParsed & 0x00FF));
				}
				else
				{
					fileReaderFinished = TRUE;
				}
				break;
			case (BINARYSTRING_FILE):
				if ( ParseBinaryStringValueFromFile(fileInputTestStream, &byteParsed) == TRUE )
				{
					*outBuffer++ = byteParsed;
				}
				else
				{
					fileReaderFinished = TRUE;
				}
				break;
			default:
				break;
		}

	}

	if (!fileReaderFinished)
	{
		return 0;
	}
	else
	{
		fclose(outputCaptureStream);
		PrintMsg("\n\n\nFile Read Finished");
		globalEndProgram = TRUE;
		return RESET_PIPELINE_DOWN_CONTROLLER;
	}
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
BYTE FileReaderInputSizeB(void)
{
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
BYTE FileReaderOutputSizeB(void)
{
	if (fileReaderFinished == TRUE)
	{
		return 0;
	}
	return 8;
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
BOOL ParseBinaryStringValueFromFile(FILE *fileStream, BYTE *byteReturned)
{
	BOOL 	invalidCharacterReceived = FALSE;
	WORD	writeValue = 0;
	BYTE	numOfNumericalDigits = 0;
	BYTE 	readByte = 0;

	
	while ( !invalidCharacterReceived && readByte != EOF )
	{
		readByte = fgetc(fileStream);
		
		if (readByte >= '0' && readByte <= '1')
		{
			writeValue *= 0x02;
			numOfNumericalDigits++;
			writeValue += readByte & 0xF;
		}
		//
		// seperators detected, commit word to memory
		// first write will not increment if there are spaces
		//
		else if ( readByte == ASCII_TAB || readByte == ASCII_SPACE
				|| readByte == ',' || readByte == ASCII_CR
				|| readByte == ASCII_LF || readByte == EOF )
		{
			//
			// Only write bytes
			//
			if (numOfNumericalDigits > 8)
			{
				invalidCharacterReceived = TRUE;
			}
			else if (numOfNumericalDigits != 0)
			{				
				numOfNumericalDigits = 0;
				*byteReturned = (BYTE)writeValue;
				return (TRUE);
			}
		}
		else
		{
			invalidCharacterReceived  = TRUE;
		}
	}
	if (invalidCharacterReceived )
	{
		PrintMsgP("\nInput File Invalid Character Received: %X", readByte);
	}
	else
	{
		PrintMsg("\nInput File EOF Reached\n");
	}
	return (FALSE);
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
BOOL ParseByteHexValueFromFile(FILE *fileStream, BYTE *byteReturned)
{
	BOOL 	invalidCharacterReceived = FALSE;
	WORD	writeValue = 0;
	BYTE	numOfNumericalDigits = 0;
	BYTE 	readByte = 0;

	
	while ( !invalidCharacterReceived && readByte != EOF )
	{
		readByte = fgetc(fileStream);
		
		if (readByte >= '0' && readByte <= '9')
		{
			writeValue *= 0x10;
			numOfNumericalDigits++;
			writeValue += readByte & 0xF;
		}
		else if ( (readByte >= 'a' && readByte <= 'f') ||
				(readByte >= 'A' && readByte <= 'F') )
		{
			writeValue *= 0x10;
			numOfNumericalDigits++;
			writeValue += (readByte & 0xF) + 9;
		}
		//
		// seperators detected, commit word to memory
		// first write will not increment if there are spaces
		//
		else if ( readByte == ASCII_TAB || readByte == ASCII_SPACE
				|| readByte == ',' || readByte == ASCII_CR
				|| readByte == ASCII_LF || readByte == EOF )
		{
			//
			// Only write bytes
			//
			if (numOfNumericalDigits > 2)
			{
				invalidCharacterReceived = TRUE;
			}
			else if (numOfNumericalDigits != 0)
			{				
				numOfNumericalDigits = 0;
				*byteReturned = (BYTE)writeValue;
				return (TRUE);
			}
		}
		else
		{
			invalidCharacterReceived  = TRUE;
		}
	}
	if (invalidCharacterReceived )
	{
		PrintMsgP("\nEncoder Tester Input File Invalid Character Received: %X", readByte);
	}
	else
	{
		PrintMsg("\nEncoder Tester Input File EOF Reached\n");
	}
	return (FALSE);
	
}

/***********************************  END  ************************************/
