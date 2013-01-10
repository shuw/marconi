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


#ifndef __PRINTMSG_H__
#define __PRINTMSG_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define SIZE_OF_PRINT_MSG_BUFFER 80


//******************************************************************************
//  E N U M S
//******************************************************************************
#define PrintMsg(a)			DebugMsg(a)
#define PrintMsgW(a,b)		DebugMsgW(a,b)
#define PrintMsgDW(a,b)		DebugMsgDW(a,b)
#define PrintMsgFloat(a,b)		DebugMsgFloat(a,b)


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
extern BYTE MessageBuffer[SIZE_OF_PRINT_MSG_BUFFER];
extern FILE *debugFileCaptureStream;

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void DebugMsg(char *StringBuffer);
void DebugMsgW(char *StringBuffer, WORD value);
void DebugMsgDW(char *StringBuffer, DWORD value);
void DebugMsgFloat(char *stringBuffer, double value);




#endif
