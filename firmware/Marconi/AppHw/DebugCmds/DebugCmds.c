//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		This file contains debug functions which are controlled by the serial port
//
//
//
//******************************************************************************


#define __DEBUGCMDS_C__


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
static void resetCmdAction(SDWORD parameters[]);



//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


//
// Top menu command list
//
ROM DEBUG_CMD_STRUCT	TopDebugCmdArray[] =
{

	{
		"p", 						//Command String
		"commands for pipeline objects",		//Command Description
		0, 							//Number of Parameters
		NULL,				//Function Associated with Command
		(BYTE *)&PipelineCmdList						//sub commands
	},
	{
		"c", 						//Command String
		"commands for controller objects",		//Command Description
		0, 							//Number of Parameters
		NULL,				//Function Associated with Command
		(BYTE *)&ControllerCmdList						//sub commands
	},	
	{
		"ec", 						//Command String
		"commands for error checker block",		//Command Description
		0, 							//Number of Parameters
		NULL,				//Function Associated with Command
		(BYTE *)&ErrorCheckCmdList						//sub commands
	},
	{
		"packet", 						//Command String
		"commands for packeting blocks",		//Command Description
		0, 							//Number of Parameters
		NULL,				//Function Associated with Command
		(BYTE *)&PacketCmdList						//sub commands
	},
	{
		"read", 
		"reads word from memory address",
		1,
		ReadCmdAction,
		NULL
	},
	{
		"write", 
		"writes word to memory address",
		2,
		WriteCmdAction,
		NULL		
	},
	{
		"rdump",
		"dumps memory address from P1 to P2",
		2,
		readDumpCmdAction,
		NULL,
	},
	{
		"wdump",
		"writes a block of memory using hex",
		2,
		writeDumpCmdAction,
		NULL		
	},
	{
		"debug",
		"debug mode will turn off all modem processes inside the main loop",
		1,
		debugModeCmdAction,
		NULL
	},
	{
		"timer",
		"calls and returns HwTimerGetCurrentTicks",
		0,
		getTimeCmdAction,
		NULL
	},
	{
		"reset",
		"resets the modem",
		0,
		resetCmdAction,
		NULL
	}
	
};


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
ROM CMD_LIST_STRUCT TopDebugCmdList =
{
	TopDebugCmdArray,
	NumOfCmdsInList(TopDebugCmdArray)
};


//******************************************************************************
//  C O D E
//******************************************************************************



//******************************************************************************
//
// FUNCTION:        N/A
//
// USAGE:             N/A
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void TestCmdAction(SDWORD parameters[])
{
	DebugMsg("Test Command Action \n\r");
}

//******************************************************************************
//
// FUNCTION:        ReadCmdAction
//
// USAGE:             Reads word from a memory address
//				
//
// INPUT:              parameter 1 - addresss
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void ReadCmdAction(SDWORD parameters[])
{

	DebugMsgW("\n\raddr 0x%X = ", parameters[0]);
	DebugMsgW("0x%X \n\r",  *(volatile WORD *)(parameters[0]));
}

//******************************************************************************
//
// FUNCTION:        WriteCmdAction
//
// USAGE:             Writes word to address
//				
//
// INPUT:              parameter 1 -address
//                         parameter 2 -address
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void WriteCmdAction(SDWORD parameters[])
{

	DebugMsgW("\n\raddr 0x%X = ", parameters[0]);
	*(volatile WORD *)(parameters[0]) = parameters[1];
	DebugMsgW("0x%X \n\r",  *(volatile WORD *)(parameters[0]));	
	
}



//******************************************************************************
//
// FUNCTION:        readDumpCmdAction
//
// USAGE:             dumps memory from location range specified by parameters
//				
//
// INPUT:              2 parameters
//
// OUTPUT:           displays to screen
//
// GLOBALS:         N/A
//
//******************************************************************************
void readDumpCmdAction(SDWORD parameters[])
{
	BYTE byteCounter = 0;
	
	volatile BYTE *startingAddrPtr 	= (BYTE *)parameters[0];
	BYTE *endingAddrPtr 		= (BYTE *)parameters[1];
		
	DebugMsg("\n\rMemory Dump");
	
	while (startingAddrPtr <= endingAddrPtr)
	{
		if (byteCounter++ == 0)
		{
			DebugMsgW("\n\r 0x%04X:  ", (DWORD)startingAddrPtr);
		}
		DebugMsgW("%04X ", *startingAddrPtr++);

		if (byteCounter > 0x7)
		{
			byteCounter = 0;
		}
	}

}

//******************************************************************************
//
// FUNCTION:        writeDumpCmdAction
//
// USAGE:             will wait for input of block data in hex form from input
//				HEX values can be seperated by spaces tabs or commas only
//				each hex represents one word
//
// INPUT:              2 parameters
//
// OUTPUT:           displays to screen
//
// GLOBALS:         N/A
//
//******************************************************************************
void writeDumpCmdAction(SDWORD parameters[])
{
	volatile WORD *addrWritePtr 	= (volatile WORD *)parameters[0];
	WORD *	endingAddrPtr 		= (WORD *)parameters[1];
	char 	uartReadByte;
	BOOL 	invalidCharacterReceived = FALSE;
	WORD	wordWriteValue = 0;
	BYTE	numOfNumericalDigits = 0;

	
	DebugMsg("\n\rMemory Write Block\n\r");
	
	while (addrWritePtr <= endingAddrPtr && !invalidCharacterReceived)
	{
		//wait for uart data to arrive
		while (HwUartRd(debugUartID, &uartReadByte, 1) == 0);

		
		if (uartReadByte >= '0' && uartReadByte <= '9')
		{
			wordWriteValue *= 0x10;
			numOfNumericalDigits++;
			wordWriteValue += uartReadByte & 0xF;
		}
		else if ( (uartReadByte >= 'a' && uartReadByte <= 'f') ||
				(uartReadByte >= 'A' && uartReadByte <= 'F') )
		{
			wordWriteValue *= 0x10;
			numOfNumericalDigits++;
			wordWriteValue += (uartReadByte & 0xF) + 9;
		}
		//
		// seperators detected, commit word to memory
		// first write will not increment if there are spaces
		//
		else if ( (uartReadByte == ASCII_TAB || uartReadByte == ASCII_SPACE
				|| uartReadByte == ',' || uartReadByte == ASCII_CR) )
		{
			if (numOfNumericalDigits > 4)
			{
				invalidCharacterReceived = TRUE;
			}
			else if (numOfNumericalDigits != 0)
			{				
				numOfNumericalDigits = 0;
				*addrWritePtr++ = wordWriteValue;
				wordWriteValue = 0;
			}
		}
		else
		{
			invalidCharacterReceived  = TRUE;
		}

	}

	if (invalidCharacterReceived)
		DebugMsg("\n\rMemory Block Write Interrupted by Invalid Character\n\r");
	else
		DebugMsg("\n\rMemory Block Write Successfull\n\r");
}


//******************************************************************************
//
// FUNCTION:        readDumpCmdAction
//
// USAGE:             dumps memory from location range specified by parameters
//				
//
// INPUT:              2 parameters
//
// OUTPUT:           displays to screen
//
// GLOBALS:         N/A
//
//******************************************************************************
void debugModeCmdAction(SDWORD parameters[])
{
	if (parameters[0] == 0)
	{
		DebugMsg("debug mode off");
		debugModeEnabled = FALSE;
	}
	else
	{
		DebugMsg("debug mode on");
		debugModeEnabled = TRUE;
	}
}

//******************************************************************************
//
// FUNCTION:        getTimeCmdAction
//
// USAGE:             return the current timer information
//
//******************************************************************************
void getTimeCmdAction(SDWORD parameters[])
{
	DebugMsgDW("\n\rCurrent Time %lu", GetCurrentTimeMS());

	
}

//******************************************************************************
//
// FUNCTION:        getTimeCmdAction
//
// USAGE:             return the current timer information
//
//******************************************************************************
static void resetCmdAction(SDWORD parameters[])
{
	debugModeEnabled = TRUE;
	ModemInit();
//	ListAllControllersAction(NULL);
	ListAllPipelinesAction(NULL);
	DebugMsg("\n\n\rtype 'help' for a list of available commands");
}

/***********************************  END  ************************************/
