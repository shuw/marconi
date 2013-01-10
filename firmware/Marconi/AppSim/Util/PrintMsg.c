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
FILE *debugFileCaptureStream;





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
void DebugMsg(char *StringBuffer)
{
	printf(StringBuffer);
	fprintf(debugFileCaptureStream, StringBuffer);

}

//******************************************************************************
//
// FUNCTION:        PrintMsgDW()
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
void DebugMsgW(char *stringBuffer, WORD value)
{	
	printf(stringBuffer, value);
	fprintf(debugFileCaptureStream, stringBuffer, value);
}

//******************************************************************************
//
// FUNCTION:        PrintMsgDW()
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
void DebugMsgDW(char *stringBuffer, DWORD value)
{	
	printf(stringBuffer, value);
	fprintf(debugFileCaptureStream, stringBuffer, value);
}

//******************************************************************************
//
// FUNCTION:        PrintMsgDW()
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
void DebugMsgFloat(char *stringBuffer, double value)
{
	printf(stringBuffer, value);
	fprintf(debugFileCaptureStream, stringBuffer, value);
}



/***********************************  END  ************************************/
