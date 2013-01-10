#define __PRINTMSG_C__

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

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
BYTE PrintMsgBuffer[SIZE_OF_PRINT_MSG_BUFFER];

//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
#ifdef VISUAL_STUDIO_MODEL
	FILE *outputCaptureStream;
#endif

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
// FUNCTION:   	PrintMsg()
//
// USAGE:       	This function prints a string
//				
//
// INPUT:       	StringBuffer
//
// OUTPUT:      	None
//
// GLOBALS:		None
//
//******************************************************************************
void PrintMsg(char *StringBuffer)
{
#ifdef VISUAL_STUDIO_MODEL
	printf(StringBuffer);
	fprintf(outputCaptureStream, StringBuffer);
#else
	while(*StringBuffer) 
	{
		if (*StringBuffer == '\n')
			HwUartWr(DEBUG_UART_ID, '\r');

		HwUartWr(DEBUG_UART_ID, *StringBuffer++);
	}
#endif
}

//******************************************************************************
//
// FUNCTION:        PrintMsgP()
//
// USAGE:             Prints a string with a wordvalue
//				
//
// INPUT:              StringBuffer
//                         WordValue
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void PrintMsgP(char *stringBuffer, WORD value)
{
#ifdef VISUAL_STUDIO_MODEL
	
	printf(stringBuffer, value);
	fprintf(outputCaptureStream, stringBuffer, value);
#else
	BYTE *PrintMsgPtr = PrintMsgBuffer;
	sprintf(PrintMsgPtr, stringBuffer, value);

	PrintMsg(PrintMsgPtr);
#endif
}

//******************************************************************************
//
// FUNCTION:        PrintMsgP()
//
// USAGE:             Prints a string with 2 double wordvalues
//				
//
// INPUT:              StringBuffer
//                         WordValue
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
/*
void PrintMsgP2(char *StringBuffer, DWORD value1, DWORD value2)
{
	BYTE *PrintMsgPtr = PrintMsgBuffer;
	sprintf(PrintMsgPtr, StringBuffer, value1, value2);

	PrintMsg(PrintMsgPtr);
}

*/
//******************************************************************************
//
// FUNCTION:        PrintMsgDirect
//
// USAGE:             Prints a message directly without interrupots
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void PrintMsgDirect(char *msg) 
{
	PrintMsg(msg);
}


/***********************************  END  ************************************/
