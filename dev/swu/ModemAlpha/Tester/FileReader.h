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


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void FileReaderInit(BYTE *outputFileNameString, FILE_FORMAT_ENUM format);
BYTE FileReader(BYTE *inBuffer, BYTE *inSize, BYTE *outBuffer, BYTE *outSize);
BYTE FileReaderInputSizeB(void);
BYTE FileReaderOutputSizeB(void);

#endif
