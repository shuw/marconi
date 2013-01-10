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


#ifndef __FILEREADER_H__
	#define __FILEREADER_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
typedef enum
{
	HEX_FILE,
	BINARYSTRING_FILE,
	BINARY_FILE,
	NUM_FILE_FORMAT
} FILE_FORMAT_ENUM;

typedef struct
{
	BYTE *fileNameString;
	FILE_FORMAT_ENUM format;
} FILE_READER_INIT_STRUCT;

typedef struct
{
	FILE * inputFileStream;
	FILE_FORMAT_ENUM fileFormat;
	WORD EOF_reached;
} FILE_READER_OBJ_STRUCT;

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void FileReaderInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD FileReader(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD FileReaderReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);


#endif
