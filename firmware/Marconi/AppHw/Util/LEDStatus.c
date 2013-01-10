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


#define __LEDSTATUS_C__


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
// FUNCTION:        SetLEDStatus
//
// USAGE:             Sets up LED events
//				
//
// INPUT:              LEDStatus - Event to trigger change in LED
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void SetLEDStatus(LED_EVENTS_ENUM LEDStatus) 
{
	switch (LEDStatus)
	{
		case (LED_DEBUG_BUSY):
			HwGpioSet(HW_GPIO_ID_A, GPIO_OPERATION_LED);
			break;
		case (LED_DEBUG_NOT_BUSY):
			HwGpioClear(HW_GPIO_ID_A, GPIO_OPERATION_LED);
			break;
		default:
			break;
	}
}

//******************************************************************************
//
// FUNCTION:        LEDHandler
//
// USAGE:             Handles LED Events which require updating (ex. flashing)
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void LEDHandler(void) 
{
	static DWORD	prevTime = 0;
	static WORD ledStatus = 0;

	if ((GetCurrentTimeMS() - prevTime) > LED_OPERATION_BLINK_PERIOD_MS)
	{
		if (ledStatus == 0)
		{
			HwGpioSet(HW_GPIO_ID_A, GPIO_OPERATION_LED);
			ledStatus = 1;
		}
		else
		{
			HwGpioClear(HW_GPIO_ID_A, GPIO_OPERATION_LED);
			ledStatus = 0;
		}
		prevTime = GetCurrentTimeMS();
	}

}

/***********************************  END  ************************************/
