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


#define __DEBUGHANDLER_C__


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


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static BOOL UartRcvDebugCommandRdy(void);
static void HelpCmd();
static BOOL CmdParser(BYTE *cmdBuffer, ROM CMD_LIST_STRUCT *cmdListToUse);


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
WORD		numOfDebugParametersFound;
SDWORD 	debugParameters[MAX_NUM_DEBUG_PARAMETERS];
BOOL 		debugModeEnabled = TRUE;
HW_UART_ID_TYPE		debugUartID = DEBUG_UART_ID;

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
static BYTE UartRcvStack[SIZE_OF_UART_RECEIVE_STACK];
static BYTE UartRcvStackFillLevel = 0;
ROM CMD_LIST_STRUCT *globalCurrentCmdListPtr;

//******************************************************************************
//  C O D E
//******************************************************************************




//******************************************************************************
//
// FUNCTION:        DebugHandler
//
// USAGE:             Handles debug commands from the UART
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         TopDebugCmdList
//
//******************************************************************************
void DebugHandler(void)
{
	//
	// Fill Input Stack
	// Design decision: only one command can be executed each iteration, TomW
	//
	if ( UartRcvDebugCommandRdy() )
	{
		SetLEDStatus(LED_DEBUG_BUSY);

		if (!CmdParser(UartRcvStack, &TopDebugCmdList))
		{
			DebugMsg("unrecognized command, try 'help'\n\r");
		}

		DisplayPrompt();
		SetLEDStatus(LED_DEBUG_NOT_BUSY);
		
	}


	
}


//******************************************************************************
//
// FUNCTION:        UartRcvDebugCommandRdy
//
// USAGE:             Check UART for new received bytes
//				
//
// INPUT:              N/A
//
// OUTPUT:           BOOL - Whether a debug command is ready to be parsed
//
// GLOBALS:         N/A
//
//******************************************************************************
BOOL UartRcvDebugCommandRdy(void)
{
	BYTE numOfReceivedBytes = HwUartRdAvail(debugUartID);
	char uartReceivedByte;


	if (numOfReceivedBytes == 0)
	{
		return FALSE;
	}
	//
	// Process each incoming character seperately
	//
	do
	{
		numOfReceivedBytes--;
		
		//
		// Uart Receive Stack is Full, so remove stack and start over
		// Leave one byte room for null character
		//
		if (UartRcvStackFillLevel >= (SIZE_OF_UART_RECEIVE_STACK - 1))
		{
			UartRcvStackFillLevel = 0;
		}

		HwUartRd(debugUartID, &uartReceivedByte, 1);

		//If Backspace character, clear character and from buffer
		if (uartReceivedByte == ASCII_BS)
		{
			if (UartRcvStackFillLevel > 0)
			{
				char temp[] = {ASCII_BS, ASCII_SPACE, ASCII_BS};

				UartRcvStackFillLevel--;
				HwUartWr(debugUartID, temp, 3);
//				HwUartWr(debugUartID, ASCII_SPACE);
//				HwUartWr(debugUartID, ASCII_BS);
			}
		}
		else
		{
			UartRcvStack[UartRcvStackFillLevel++] = uartReceivedByte;

			//Echo Valid received Characters
			if (uartReceivedByte >= 0x20 && uartReceivedByte <= 0x7E)
			{
				char temp = uartReceivedByte;
				HwUartWr(debugUartID, &temp, 1);
			}
		}
		

		if (uartReceivedByte == ASCII_CR)
		{
			//Replaces Carriage Return in Buffer with NULL
			UartRcvStack[--UartRcvStackFillLevel] = ASCII_NULL;			
			UartRcvStackFillLevel = 0; //frees stack for next command
			DebugMsg("\n\r");
/*
			DebugMsg("Cmd Received: ");
			DebugMsg(UartRcvStack);
			DebugMsg("\n\r");
*/
			return TRUE;
		}
		else
		{
			return FALSE;
		}

	}	while ( numOfReceivedBytes > 0);

}



//******************************************************************************
//
// FUNCTION:        CmdParser
//
// USAGE:             Parses commands to find debug actions
//				
//
// INPUT:              cmdBuffer - buffer containing input string
//
// OUTPUT:           N/A
//
// GLOBALS:         debugParameters, globalCurrentCmdListPtr
//
//******************************************************************************
static BOOL CmdParser(BYTE *cmdBuffer, ROM CMD_LIST_STRUCT *cmdListToUse)
{
	BYTE cmdSearchCounter = 0;
	BOOL cmdStringMatchFound = FALSE; 
	
	BYTE *cmdBufferPtr;
	char *cmdString;
	ROM DEBUG_CMD_STRUCT *debugCmdStructPtr;
	BYTE sizeOfDebugList;
	
	globalCurrentCmdListPtr = cmdListToUse;
	sizeOfDebugList = cmdListToUse->numOfCmdsInList;
	

	//Look for help command
	cmdBufferPtr = cmdBuffer;
	if (*cmdBufferPtr++ == 'h' &&
		*cmdBufferPtr++ == 'e' &&
		*cmdBufferPtr++ == 'l' &&
		*cmdBufferPtr++ == 'p' )
	{
		HelpCmd();
		return TRUE;
	}


	while (cmdSearchCounter < sizeOfDebugList && !cmdStringMatchFound)
	{
		BOOL byteMatchContinue = TRUE;

		cmdBufferPtr = cmdBuffer;
		debugCmdStructPtr = &(cmdListToUse->debugCmdList[cmdSearchCounter]);

		
		cmdString = debugCmdStructPtr->cmdString;
/*
		DebugMsg("Matching Command: ");
		DebugMsg(cmdString);
		DebugMsg("\n\r");
*/
		do 
		{
			if (*cmdString != ASCII_NULL)
			{
				//Case sensitivity is not matched
				if (( (*cmdBufferPtr >= 'a' && *cmdBufferPtr <= 'z') ||
					(*cmdBufferPtr >= 'A' && *cmdBufferPtr <= 'Z')  ) &&

					( (*cmdString >= 'a' && *cmdString <= 'z') ||
					(*cmdString >= 'A' && *cmdString <= 'Z')  ) )
					
				{
					if ( (*cmdString & 0x1F) != (*cmdBufferPtr & 0x1F) )
					{
						byteMatchContinue = FALSE;
					}
				}			
				else if ( *cmdString != *cmdBufferPtr )
					byteMatchContinue = FALSE;
				
				cmdString++;
				cmdBufferPtr++;
			}
			//
			//CmdString Terminated with all matching
			//
			else 
			{
				byteMatchContinue = FALSE;

				//CmdBuffer Terminated with all matching
				if (*cmdBufferPtr == ASCII_SPACE || *cmdBufferPtr == NULL)
				{
					//
					// MATCH FOUND
					//
					cmdStringMatchFound = TRUE;
					cmdBuffer = cmdBufferPtr;
				}
					
			}
		} while ( byteMatchContinue && cmdBufferPtr != NULL);

		cmdSearchCounter++;		
	}

	//Skip Spaces, look for space after command
	while (*cmdBuffer == ASCII_SPACE)
		cmdBuffer++;


	//
	// Match found
	//
	if (cmdStringMatchFound)
	{
		BYTE numOfCmdParameters = debugCmdStructPtr->numOfCmdParameters;
		BYTE numOfCmdParametersFound = 0;
		SWORD parsedParameter;
		BOOL invalidNumeralFound = FALSE;


		//
		// Recursive parsing of subcommands
		//
		if (debugCmdStructPtr->subCmdList != NULL)
		{
			if (CmdParser(cmdBuffer, (CMD_LIST_STRUCT*)debugCmdStructPtr->subCmdList) == TRUE)
			{
				return TRUE;
			}
			else
			{
				DebugMsg("the command is not complete, please type 'cmd help' for list \n\r");
			}
		}

		//
		// Now Parse Numerical Arguments
		//			
		while ( (numOfCmdParametersFound < MAX_NUM_DEBUG_PARAMETERS) && (*cmdBuffer != NULL) && !invalidNumeralFound )
		{
			parsedParameter = 0;
			
			//parse hex
			if ( *cmdBuffer == '0'  && *(cmdBuffer + 1) == 'x')
			{
				cmdBuffer += 2;

				while (*cmdBuffer != NULL && *cmdBuffer != ASCII_SPACE)
				{
					parsedParameter *= 16;
					
					if (*cmdBuffer >= '0' && *cmdBuffer <= '9')
					{
						parsedParameter += (*cmdBuffer & 0xF);
					}
					else if ( (*cmdBuffer >= 'a'  && *cmdBuffer <= 'f') ||
							(*cmdBuffer >= 'A' && *cmdBuffer <= 'F') )
					{
						parsedParameter += (*cmdBuffer & 0xF) + 9;
					}
					// Invalid Hex Numeral Found, Let Parameter Parser Terminate
					else
					{						
						invalidNumeralFound = TRUE;
					}
					cmdBuffer++;
				}
			}
			else if  (*cmdBuffer =='b')
			{
				cmdBuffer++;
				while (*cmdBuffer != NULL && *cmdBuffer != ASCII_SPACE)
				{
					parsedParameter = parsedParameter << 1;
					if (*cmdBuffer == '0' || *cmdBuffer == '1')
					{
						parsedParameter += *cmdBuffer & 0x1;
					}
					// Invalid Binary Numeral Found, Let Parameter Parser Terminate
					else
					{
						invalidNumeralFound = TRUE;
					}
					cmdBuffer++;
				}
			}
			else 
			{
				BOOL isNegative = FALSE;
				
				if (*cmdBuffer == '-')
				{
					cmdBuffer++;
					isNegative = TRUE;
				}
				
				while (*cmdBuffer != NULL && *cmdBuffer != ASCII_SPACE)
				{
					parsedParameter = parsedParameter * 10;
					if (*cmdBuffer >= '0' && *cmdBuffer <= '9')
					{
						parsedParameter += *cmdBuffer & 0xF;
					}
					// Invalid Decimall Found, Let Parameter Parser Terminate
					else
					{
						invalidNumeralFound = TRUE;
					}
					cmdBuffer++;
				}

				if (isNegative)
				{
					parsedParameter *= -1;
				}
			}
			

			debugParameters[numOfCmdParametersFound++] = parsedParameter;

			//Skip Spaces, look for space after command
			while (*cmdBuffer == ASCII_SPACE)
				cmdBuffer++;
 		}
		

		//
		// Now Command Function
		//
		if ( ( (numOfCmdParameters == numOfCmdParametersFound)  ||
				( numOfCmdParameters == NUMOFCMDPARAMETERS_CANBEANYSIZE))
				&& !invalidNumeralFound)
		{
			numOfDebugParametersFound = numOfCmdParametersFound;
			if (debugCmdStructPtr->CmdFunctionPtr != NULL)
				debugCmdStructPtr->CmdFunctionPtr(debugParameters);
			return (TRUE);
		}
	}
	return (FALSE);

}

//******************************************************************************
//
// FUNCTION:        HelpCmdAction
//
// USAGE:             Provides a list of commands
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
static void HelpCmd()
{
	BYTE cmdCounter;
	ROM DEBUG_CMD_STRUCT *debugCmdStructPtr;

	DebugMsg("Command List");
	for (cmdCounter = 0; cmdCounter < globalCurrentCmdListPtr->numOfCmdsInList; cmdCounter++)
	{
		debugCmdStructPtr = &(globalCurrentCmdListPtr->debugCmdList[cmdCounter]);
		DebugMsg("\n\r");
		DebugMsg(debugCmdStructPtr->cmdString);
		if (debugCmdStructPtr->numOfCmdParameters == NUMOFCMDPARAMETERS_CANBEANYSIZE)
		{
			DebugMsg("\n\r    # of parameters can be any size");
		}
		else
			DebugMsgW("\n\r   # of parameters: %d", debugCmdStructPtr->numOfCmdParameters);
		DebugMsg("\n\r   description: ");
		DebugMsg(debugCmdStructPtr->cmdDescription);
	}

	
}

//******************************************************************************
//
// FUNCTION:        DisplayPrompt
//
// USAGE:             Displays command prompt when debug action is finished
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void DisplayPrompt(void)
{
	DebugMsg("\n\rready> ");
}


/***********************************  END  ************************************/
