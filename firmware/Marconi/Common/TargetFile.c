//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Provides file read and write support that works in both PC simulation
// and TI DSP simulator targets. 
//
//******************************************************************************


#include <stdio.h>
#include "GlobalDefs.h"


void TargetFileRead(void *buf, size_t size, int count, FILE *fp) {
    int i;
    WORD *b = (WORD *)buf;

    for(i = 0; i < count * (int)size / (int)sizeof(WORD); ++i) {
        // We must read two chars to fill one WORD
        b[i] = (WORD)fgetc(fp);
        b[i] |= fgetc(fp) << 8;
    }
}


void TargetFileWrite(void *buf, size_t size, int count, FILE *fp) {
    int i;
    WORD *b = (WORD *)buf;

    for(i = 0; i < count * (int)size / (int)sizeof(WORD); ++i) {
        // We must write two chars to output one WORD
        fputc(b[i], fp);
        fputc(b[i] >> 8, fp);
    }
}
