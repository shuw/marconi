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


#ifndef __DEBUGHANDLER_H__
#define __DEBUGHANDLER_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//
// The parameters can be anything
//
#define NUMOFCMDPARAMETERS_CANBEANYSIZE		0x3F

#define NumOfCmdsInList(a)  (sizeof(a) / sizeof(DEBUG_CMD_STRUCT))

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define SIZE_OF_UART_RECEIVE_STACK  		50
#define DEBUG_UART_ID						HW_UART_ID_B
#define APP_UART_ID							HW_UART_ID_A

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern ROM CMD_LIST_STRUCT *globalCurrentCmdListPtr;
extern SDWORD 				debugParameters[MAX_NUM_DEBUG_PARAMETERS];
extern BOOL 					debugModeEnabled;
extern WORD					numOfDebugParametersFound;
extern HW_UART_ID_TYPE		debugUartID;

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void DebugHandler(void);
void DisplayPrompt(void);



#endif
