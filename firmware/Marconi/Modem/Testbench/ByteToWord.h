//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Converts individual BYTES (1 per WORD) into WORD-packed BYTES.
//
//******************************************************************************


#ifndef __BYTETOWORD_H__
#define __BYTETOWORD_H__


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void ByteToWordInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD ByteToWord(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD ByteToWordReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);

#endif
