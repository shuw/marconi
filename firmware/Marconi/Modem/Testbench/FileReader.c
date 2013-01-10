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
#include "TargetFile.h"


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
BOOL ParseByteHexValueFromFile(FILE *fileStream, WORD *byteReturned);
BOOL ParseBinaryStringValueFromFile(FILE *fileStream, WORD *byteReturned);
	
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
void FileReaderInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	FILE_READER_INIT_STRUCT *fileReaderInitStruct = (FILE_READER_INIT_STRUCT *)BlockInitStruct;
	FILE_READER_OBJ_STRUCT *fileReaderObjStruct = (FILE_READER_OBJ_STRUCT *)BlockObjStruct;

    if(fileReaderObjStruct->inputFileStream) {
        // Close any previously opened file upon resetting (when we reset multiple times)
        fclose(fileReaderObjStruct->inputFileStream);
    }
	
	(fileReaderObjStruct->EOF_reached) = FALSE;

	fileReaderObjStruct->EOF_reached = FALSE;
		
	// open a file for reading
	if (fileReaderInitStruct->format == BINARY_FILE)
	{
		(fileReaderObjStruct->inputFileStream) = fopen((char *)fileReaderInitStruct->fileNameString, "rb");		
	}
	else
	{
		(fileReaderObjStruct->inputFileStream) = fopen((char *)fileReaderInitStruct->fileNameString , "r");
	}
	(fileReaderObjStruct->fileFormat) = fileReaderInitStruct->format;

    if(fileReaderObjStruct->inputFileStream == 0) {
        printf("Error: could not open input file: %s\n", fileReaderInitStruct->fileNameString);
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
// TODO: Make a generic tester based around controller

WORD FileReader(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	WORD byteParsed = 0;
	WORD outSizeB = *outSize;
	FILE_READER_OBJ_STRUCT *fileReaderObjStruct = (FILE_READER_OBJ_STRUCT *)BlockObjStruct;
	
	//
	// Encoder Test Code
	//
	while ( !(fileReaderObjStruct->EOF_reached) && outSizeB-- > 0)
	{
		switch ((fileReaderObjStruct->fileFormat))
		{
			case (HEX_FILE):
				
				if ( ParseByteHexValueFromFile((fileReaderObjStruct->inputFileStream), &byteParsed) == TRUE )
				{
					*outBuffer++ = byteParsed;

		//			PrintMsgDW("\nIn Buffer %02X", (WORD)(byteParsed & 0x00FF));
				}
				else
				{
					(fileReaderObjStruct->EOF_reached) = TRUE;
				}
				break;
			case (BINARYSTRING_FILE):
				if ( ParseBinaryStringValueFromFile((fileReaderObjStruct->inputFileStream), &byteParsed) == TRUE )
				{
					*outBuffer++ = byteParsed;
				}
				else
				{
					(fileReaderObjStruct->EOF_reached) = TRUE;
				}
				break;
			case (BINARY_FILE):
				{
#if (BITS_IN_WORD == 8)
					*outBuffer++ = fgetc((fileReaderObjStruct->inputFileStream));
#elif (BITS_IN_WORD == 16)
					//
					// Reads big endian
					//
                    TargetFileRead(outBuffer, sizeof(WORD), 1, fileReaderObjStruct->inputFileStream);
                    outBuffer++;
#endif

					if (feof((fileReaderObjStruct->inputFileStream)))
					{
						(fileReaderObjStruct->EOF_reached) = TRUE;
//						DebugMsg("\nInput File EOF Reached\n");
					}
				}
				break;
			default:
				break;
		}

	}

    *outSize -= outSizeB + 1;

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
WORD FileReaderReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	FILE_READER_OBJ_STRUCT *fileReaderObjStruct = (FILE_READER_OBJ_STRUCT *)BlockObjStruct;

	if ((fileReaderObjStruct->EOF_reached) == TRUE || *outputSize < 8)
	{
		return FALSE;
	}
	*outputSize = 8;
	return TRUE;
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
BOOL ParseBinaryStringValueFromFile(FILE *fileStream, WORD *byteReturned)
{
	BOOL 	invalidCharacterReceived = FALSE;
	WORD	writeValue = 0;
	WORD	numOfNumericalDigits = 0;
	SWORD 	readByte = 0;

	
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
			if (numOfNumericalDigits > BITS_IN_WORD)
			{
				invalidCharacterReceived = TRUE;
			}
			else if (numOfNumericalDigits != 0)
			{				
				numOfNumericalDigits = 0;
				*byteReturned = writeValue;
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
		DebugMsgW("\nInput File Invalid Character Received: %X", readByte);
	}
	else
	{
		DebugMsg("\nInput File EOF Reached\n");
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
BOOL ParseByteHexValueFromFile(FILE *fileStream, WORD *byteReturned)
{
	BOOL 	invalidCharacterReceived = FALSE;
	WORD	writeValue = 0;
	WORD	numOfNumericalDigits = 0;
	SWORD 	readByte = 0;

	
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
			if (numOfNumericalDigits > (BITS_IN_WORD / 4))
			{
				invalidCharacterReceived = TRUE;
			}
			else if (numOfNumericalDigits != 0)
			{				
				numOfNumericalDigits = 0;
				*byteReturned = writeValue;
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
		DebugMsgW("\nEncoder Tester Input File Invalid Character Received: %X", readByte);
	}
	else
	{
		DebugMsg("\nEncoder Tester Input File EOF Reached\n");
	}
	return (FALSE);
	
}

/***********************************  END  ************************************/
