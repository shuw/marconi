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

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
char MessageBuffer[SIZE_OF_PRINT_MSG_BUFFER];

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


// NOTE: if the string length is equal to the buffer length, we have
// still overflowed since the NULL terminator is not counted by strlen()
#define CHECK_MESSAGE_BUFFER_LENGTH \
    if(strlen(MessageBuffer) >= SIZE_OF_PRINT_MSG_BUFFER) { \
        DebugMsg("ERROR: MessageBuffer[] size exceeded.\r\n"); \
        DebugMsg("Program execution halted!\r\n"); \
        while(1); \
    }


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
	WORD stringLength = strlen(StringBuffer);
	while (HwUartWrAvail(APP_UART_ID) < stringLength);
		
	HwUartWr(APP_UART_ID, StringBuffer, stringLength);
}

//******************************************************************************
//
// FUNCTION:        PrintMsgW()
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
void PrintMsgW(char *stringBuffer, WORD value)
{
	char *PrintMsgWtr = MessageBuffer;
	sprintf(PrintMsgWtr, stringBuffer, value);
    CHECK_MESSAGE_BUFFER_LENGTH;

	PrintMsg(PrintMsgWtr);
}



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
	WORD stringLength = strlen(StringBuffer);
	while (HwUartWrAvail(debugUartID) < stringLength);
		
	HwUartWr(debugUartID, StringBuffer, stringLength);
}

//******************************************************************************
//
// FUNCTION:        PrintMsgW()
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
	char *PrintMsgWtr = MessageBuffer;
	sprintf(PrintMsgWtr, stringBuffer, value);
    CHECK_MESSAGE_BUFFER_LENGTH;

	DebugMsg(PrintMsgWtr);
}




//******************************************************************************
//
// FUNCTION:        PrintMsgW()
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
	char *PrintMsgWtr = MessageBuffer;
	sprintf(PrintMsgWtr, stringBuffer, value);
    CHECK_MESSAGE_BUFFER_LENGTH;

	DebugMsg(PrintMsgWtr);
}

//******************************************************************************
//
// FUNCTION:        PrintMsgW()
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
void DebugMsgFloat(char *stringBuffer, float value)
{
	char *PrintMsgWtr = MessageBuffer;
	sprintf(PrintMsgWtr, stringBuffer, value);
    CHECK_MESSAGE_BUFFER_LENGTH;

	DebugMsg(PrintMsgWtr);
}


/***********************************  END  ************************************/
