//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Interface for FSK modulator.
//
//******************************************************************************


#ifndef __FSK_MODULATOR_H__
#define __FSK_MODULATOR_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************


//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


typedef struct {
    WORD savedBits;
    WORD bitBuffer;
} FSK_MODULATOR_STATE_TYPE;


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


void FSKModulatorInit(BYTE *blockInitStruct, BYTE *blockObjStruct);
WORD FSKModulator(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *blockObjStruct);
WORD FSKModulatorReady(WORD *inSize, WORD *outSize, BYTE *blockObjStruct);


#endif
