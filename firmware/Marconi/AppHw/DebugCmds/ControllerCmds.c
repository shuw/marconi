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


#define __CONTROLLERCMDS_C__


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
static void DumpBufferAction(SDWORD parameters[]);
static void SetSNRforAWGNAction(SDWORD parameters[]);
static void SetControllerDebugAction(SDWORD parameters[]);
static void SetControllerDisableAction(SDWORD parameters[]);
static void SetControllerPassThruAction(SDWORD parameters[]);
//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
static ROM DEBUG_CMD_STRUCT	controllerCmdArray[] =
{
	{
		"list", 
		"lists IDs of all instantiated controllers",
		0,
		ListAllControllersAction,
		NULL
	},
	{
		"dump", 
		"dumps the buffer of the selected controller\n\r \
	param0 - controller ID",
		1,
		DumpBufferAction,
		NULL
	},
	{
		"setSNR",
		"PARAM1 - SNR*100 (no decimals)",
		1,
		SetSNRforAWGNAction,
		NULL	
	},
	{
		"debug", 
		"turns on/off the controller debug mode\n\r  \
	param0 - controller ID \n\r \
	param1 - debug on/off",
		2,
		SetControllerDebugAction,
		NULL
	},
	{
		"disable", 
		"turns on/off the disable of the controller \n\r  \
	param0 - controller ID \n\r \
	param1 - disable on/off",
		2,
		SetControllerDisableAction,
		NULL
	},
	{
		"pass", 
		"turns on/off the pass through of the controller \n\r  \
	param0 - controller ID \n\r \
	param1 - passthrough on/off",
		2,
		SetControllerPassThruAction,
		NULL
	}

};



//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
ROM CMD_LIST_STRUCT ControllerCmdList =
{
	controllerCmdArray,
	NumOfCmdsInList(controllerCmdArray)
};



//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  C O D E
//******************************************************************************


//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
void ListAllControllersAction(SDWORD parameters[])
{
	BYTE count;

	DebugMsg("		\n\r-- Controllers Instantiation List --");
	for (count = 0; count <NUM_OF_DATA_CONTROLLERS; count++)
	{
		DebugMsgW("\n\rID #%d - ", count);
		DebugMsg(DataControllersInstantiationList[count].ControllerSelect->nameString);
	}
	
}

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void DumpBufferAction(SDWORD parameters[])
{
	DATA_CONTROLLER * ControllerPtr = DataControllersInstantiationList[parameters[0]].ControllerSelect;
	WORD count;
	WORD * bufferPtr;
	
	DebugMsg(ControllerPtr->nameString);
	DebugMsgW("\n\rFifo Fill Level = %d \n\r", ControllerPtr->fifoFillLevel);



	DebugMsg("\n\rDump of entire buffer\n\r");
	bufferPtr = ControllerPtr->fifoStartAddress;
	for (count = 0; count < ControllerPtr->fifoSize; count++)
	{
		DebugMsgW("0x%04X ", *bufferPtr++);
	}
	
	DebugMsg("\n\r\n\rDump of filled buffer\n\r");
	bufferPtr = ControllerPtr->fifoPopPtr;
	for (count = 0; count < ControllerPtr->fifoFillLevel; count++)
	{
		DebugMsgW("0x%04X ", *bufferPtr++);

		if (bufferPtr >= ControllerPtr->fifoEndAddress)
		{
			bufferPtr = ControllerPtr->fifoStartAddress;
		}
	}

}

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void SetSNRforAWGNAction(SDWORD parameters[])
{
	AWGN_CHANNEL_OBJ_STRUCT * AWGNObjPtr = 
		(AWGN_CHANNEL_OBJ_STRUCT *)AWGNChannelController.BlockObjStruct;
	AWGNObjPtr->SignalToNoiseDB = (double)parameters[0] / 100;
	DebugMsgFloat("\n\rAWGN SNR dB Level = %f", AWGNObjPtr->SignalToNoiseDB);
	
}

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void SetControllerDebugAction(SDWORD parameters[])
{
	if (parameters[1] == 1)
	{
		DataControllersInstantiationList[parameters[0]].ControllerSelect->optionFlags |= CONTROLLER_OPTIONS_PRINT_DEBUG;
	}
	else
	{
		DataControllersInstantiationList[parameters[0]].ControllerSelect->optionFlags &= ~CONTROLLER_OPTIONS_PRINT_DEBUG;
	}
}

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void SetControllerDisableAction(SDWORD parameters[])
{
	if (parameters[1] == 1)
	{
		DataControllersInstantiationList[parameters[0]].ControllerSelect->optionFlags |= CONTROLLER_OPTIONS_DISABLE;
	}
	else
	{
		DataControllersInstantiationList[parameters[0]].ControllerSelect->optionFlags &= ~CONTROLLER_OPTIONS_DISABLE;
	}
}

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void SetControllerPassThruAction(SDWORD parameters[])
{
	if (parameters[1] == 1)
	{
		DataControllersInstantiationList[parameters[0]].ControllerSelect->optionFlags |= CONTROLLER_OPTIONS_PASSTHRU;
	}
	else
	{
		DataControllersInstantiationList[parameters[0]].ControllerSelect->optionFlags &= ~CONTROLLER_OPTIONS_PASSTHRU;
	}
}



/***********************************  END  ************************************/
