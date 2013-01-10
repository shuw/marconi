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
void FileWriterInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	FILE_WRITER_INIT_STRUCT *FileWriterInitStruct = (FILE_WRITER_INIT_STRUCT *)BlockInitStruct;
	FILE_WRITER_OBJ_STRUCT *FileWriterObjStruct = (FILE_WRITER_OBJ_STRUCT *)BlockObjStruct;

    if(FileWriterObjStruct->fileOutstream) {
        // Close any previously opened file upon resetting (when we reset multiple times)
        fclose(FileWriterObjStruct->fileOutstream);
    }

    if(FileWriterInitStruct->format == BINARY_FILE) {
        FileWriterObjStruct->fileOutstream =  fopen((char *)FileWriterInitStruct->outputFileNameString , "wb+");
    }
    else {
        FileWriterObjStruct->fileOutstream =  fopen((char *)FileWriterInitStruct->outputFileNameString , "w+");
    }
    if(FileWriterObjStruct->fileOutstream == 0) {
        printf("Error: could not open output file: %s\n", FileWriterInitStruct->outputFileNameString);
    }
	FileWriterObjStruct->fileFormat = FileWriterInitStruct->format;
	FileWriterObjStruct->counter = 0;
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
WORD FileWriter(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	FILE_WRITER_OBJ_STRUCT *FileWriterObjStruct = (FILE_WRITER_OBJ_STRUCT *)BlockObjStruct;
	
	FILE * fileOutputtestStream = FileWriterObjStruct->fileOutstream;
	WORD inputSize = *inSize;
	WORD bitCounter = BITS_IN_WORD;

	//
	// File Writer will pass through data to the next block
	//
	WORD *inBufferPtr = inBuffer;
	WORD *outBufferPtr = outBuffer;
	WORD outputSize = *outSize;
	WORD counter = FileWriterObjStruct->counter;

	while (outputSize-- > 0)
	{
		*outBufferPtr++ = *inBufferPtr++;
	}
	
	
	while ( inputSize-- > 0)
	{
		counter ++;

		switch (FileWriterObjStruct->fileFormat)
		{
			case (HEX_FILE):
				//
				// Big Endian Printout
				//
				fprintf(fileOutputtestStream, "%02X ", (WORD)(*inBuffer++ & 0xFF00));
				fprintf(fileOutputtestStream, "%02X ", (WORD)(*inBuffer++ & 0x00FF));

		//		DebugMsgW("\nOut Buffer: %02X", (WORD)(*inBuffer & 0x00FF));

				if (counter == 16)
				{
					counter  = 0;
//					fprintf(fileOutputtestStream, "\n");
				}
				break;
			case (BINARYSTRING_FILE):
				bitCounter = BITS_IN_WORD;
#if (BITS_IN_WORD == 8)				
				while (bitCounter-- > 0)
				{
					fprintf(fileOutputtestStream, "%01d", (WORD)(*inBuffer & 0x0080) >> 7);
					*inBuffer = *inBuffer << 1;
				}
#endif

#if (BITS_IN_WORD == 16)
				while (bitCounter-- > 0)
				{
					fprintf(fileOutputtestStream, "%01d", (WORD)(*inBuffer & 0x8000) >> 15);
					*inBuffer = *inBuffer << 1;
				}

#endif
				fprintf(fileOutputtestStream, " ");
				inBuffer++;

				if (counter == 4)
				{
					counter  = 0;
					fprintf(fileOutputtestStream, "\n");
				}
				break;
			case (BINARY_FILE):
#if (BITS_IN_WORD == 8)
				fputc( *inBuffer++, fileOutputtestStream);
#elif (BITS_IN_WORD == 16)

				//fputc( (*inBuffer & 0xFF), fileOutputtestStream);
				//fputc( (*inBuffer >> 8), fileOutputtestStream);
                TargetFileWrite(inBuffer, sizeof(WORD), 1, fileOutputtestStream);
                inBuffer++;
#else
				DebugMsg("Unrecognized WORD size in FileWriter.c");
#endif
				break;
			default:
				break;
		}
	}


	FileWriterObjStruct->counter = counter;
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
WORD FileWriterReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
    if(*inputSize >= 1) {
		return TRUE;
    }

    return FALSE;
}





/***********************************  END  ************************************/
