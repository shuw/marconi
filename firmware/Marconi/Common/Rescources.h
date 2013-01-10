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


#ifndef __RESCOURCES_H__
	#define __RESCOURCES_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
//#define DATACONTROLLER_DEBUG_ON


//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  M A K E   S U R E   G E N E R A L   B U F F E R   S I Z E   I S   B I G   E N O U G H
//******************************************************************************
#define SIZE_OF_GENERAL_USE_TEMP_BUFFER			1000

//This is an alternative to pointing to NULL, which causes exceptions sometimes during Visual
//Studio compile
#define ZERO_PTR									&ZeroVariable


//******************************************************************************
//  C O N T R O L L E R   B U F F E R   S I Z E S
//******************************************************************************
#define SIZEOF_VDECODER_BUFFER                      32
#define SIZEOF_VENCODER_BUFFER                      32
#define	SIZEOF_TESTMODULATOR_BUFFER                 32
#define	SIZEOF_EC_SOURCE_BUFFER                     32
#define SIZEOF_EC_DESTINATION_BUFFER                32
#define SIZEOF_AWGNCHANNEL_BUFFER                   64
#define	SIZEOF_PACKET_CREATER_BUFFER				32
#define SIZEOF_PACKET_PARSER_BUFFER					64
#define SIZEOF_FSK_MODULATOR_BUFFER                 8
#define SIZEOF_FSK_DEMODULATOR_BUFFER               256 
#define SIZEOF_WORD_TO_BYTE_BUFFER                  8
#define SIZEOF_BYTE_TO_WORD_BUFFER                  8
#define SIZEOF_TXGATE_BUFFER                        40
#define SIZEOF_RXGATE_BUFFER                        40 
#define SIZEOF_DATACHECK_BUFFER			40

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern WORD GlobalTempBuffer[SIZE_OF_GENERAL_USE_TEMP_BUFFER];
extern DWORD ZeroVariable;



//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************




#endif
