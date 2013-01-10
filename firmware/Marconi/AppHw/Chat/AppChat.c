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


#define __APPCHAT_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"
#include <string.h>

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
static WORD ChatUartRcvFifo[SIZE_OF_CHAT_MSG_STACK];
static WORD chatPromptShown = FALSE;
static WORD bytesRemaining = 0;
static ROM char *msgReceivedStr1 = "\n\r> '";
static ROM char *msgReceivedStr2 = "' \n\r";

//******************************************************************************
//  C O D E
//******************************************************************************


//******************************************************************************
//
//	App Chat Initialization Block
//
//******************************************************************************
void AppChatInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	CHAT_OBJ_STRUCT * ChatObjStruct = (CHAT_OBJ_STRUCT *)BlockObjStruct;
	ChatObjStruct->state = WAITING_FOR_NEXT_MSG;
    ChatObjStruct->copyStartIndex = 0;
}


//******************************************************************************
//
//	App Chat Execution Block
//
//******************************************************************************
WORD AppChat(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	CHAT_OBJ_STRUCT * ChatObjStruct = (CHAT_OBJ_STRUCT *)BlockObjStruct;

	switch (ChatObjStruct->state)
	{
		case (WAITING_FOR_NEXT_MSG):
			if (ChatMsgRdy())
			{
				//
				// Pads the chat msg to always fit into a packet neatly
				//
				ChatObjStruct->msgPadSize = (DEFAULT_PACKET_DATA_SIZE_B - 
							(bytesRemaining % DEFAULT_PACKET_DATA_SIZE_B))
							% DEFAULT_PACKET_DATA_SIZE_B;
				ChatObjStruct->state = SEND_MSG_HEADER;
				ChatObjStruct->copyStartIndex = 0;
			}
			break;
		case (SEND_MSG_HEADER):
			if (*outSize >= strlen(msgReceivedStr1))
			{
				*outSize = strlen(msgReceivedStr1);
				memcpy(outBuffer, msgReceivedStr1, *outSize);
				
				ChatObjStruct->state = SENDING_MSG;
				return RETURN_OK;
			}			
			break;
		case (SEND_MSG_ENDER):
			if (*outSize >= strlen(msgReceivedStr2))
			{
				*outSize = strlen(msgReceivedStr2);
				memcpy(outBuffer, msgReceivedStr2, *outSize);
				
				ChatObjStruct->state = SEND_MSG_PAD;
				return RETURN_OK;
			}			
			break;			
		case (SENDING_MSG):
			*outSize = MIN(*outSize, bytesRemaining);
			memcpy(outBuffer, 
				ChatUartRcvFifo + ChatObjStruct->copyStartIndex, 
				*outSize);

			ChatObjStruct->copyStartIndex += *outSize;
			bytesRemaining -= *outSize;

			if (bytesRemaining == 0)
			{
				chatPromptShown = FALSE;				
				ChatObjStruct->state = SEND_MSG_ENDER;
			}
			return RETURN_OK;
		case (SEND_MSG_PAD):
			if (*outSize >= ChatObjStruct->msgPadSize)
			{
				WORD i;
				*outSize = ChatObjStruct->msgPadSize;
				for (i = 0; i < ChatObjStruct->msgPadSize; i++)
				{
					*outBuffer++ = 0;
				}
				ChatObjStruct->state = WAITING_FOR_NEXT_MSG;
				return RETURN_OK;
			}
			break;
	}
	
	return RETURN_CONTROLLER_BLOCK_IDLE;
}

//******************************************************************************
//
//	App Chat Ready Block
//
//******************************************************************************
WORD AppChatReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	return TRUE;
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
BOOL ChatMsgRdy(void)
{
	BYTE numOfReceivedBytes = HwUartRdAvail(APP_UART_ID);
	char uartReceivedByte;


	if (numOfReceivedBytes == 0)
	{
		return FALSE;
	}

	if (!chatPromptShown)
	{
		PrintMsg("\n\rChat> ");
		chatPromptShown = 1;
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
		if (bytesRemaining >= (SIZE_OF_CHAT_MSG_STACK - 1))
		{
			bytesRemaining = SIZE_OF_CHAT_MSG_STACK - 1;
			return TRUE;
//			bytesRemaining = 0;
		}

		HwUartRd(APP_UART_ID, &uartReceivedByte, 1);

		//If Backspace character, clear character and from buffer
		if (uartReceivedByte == ASCII_BS)
		{
			if (bytesRemaining > 0)
			{
				char temp[] = {ASCII_BS, ASCII_SPACE, ASCII_BS};

				bytesRemaining--;
				HwUartWr(APP_UART_ID, temp, 3);
			}
		}
		else
		{
			ChatUartRcvFifo[bytesRemaining++] = uartReceivedByte;

			//Echo Valid received Characters
			if (uartReceivedByte >= 0x20 && uartReceivedByte <= 0x7E)
			{
				char temp = uartReceivedByte;
				HwUartWr(APP_UART_ID, &temp, 1);
			}
		}
		

		if (uartReceivedByte == ASCII_CR)
		{
			//Replaces Carriage Return in Buffer with NULL
			ChatUartRcvFifo[--bytesRemaining] = ASCII_NULL;			
//			bytesRemaining = 0; //frees stack for next command
			PrintMsg("\n\r");
			return TRUE;
		}
		else
		{
			return FALSE;
		}

	}	while ( numOfReceivedBytes > 0);

}



/***********************************  END  ************************************/
