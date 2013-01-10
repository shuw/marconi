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


#ifndef __FILEWRITER_H__
	#define __FILEWRITER_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef struct
{
	BYTE *outputFileNameString;
	FILE_FORMAT_ENUM format;
} FILE_WRITER_INIT_STRUCT;


typedef struct
{
	FILE * fileOutstream;
	FILE_FORMAT_ENUM fileFormat;
	WORD counter;
} FILE_WRITER_OBJ_STRUCT;

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void FileWriterInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD FileWriter(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD FileWriterReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);


#endif
