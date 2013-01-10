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


#ifndef __CONTROLLERS_H__
	#define __CONTROLLERS_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef enum
{
	DATA_CHECK_CONTROLLER_ID,
	CHAT_CONTROLLER_ID,
	FSK_MODULATOR_CONTROLLER_ID,
	FSK_DEMODULATOR_CONTROLLER_ID,
    BURST_TX_CONTROLLER_ID,
    BURST_RX_CONTROLLER_ID,
	VENCODER_CONTROLLER_ID,
	VDECODER_CONTROLLER_ID,
	TEST_MODULATOR_CONTROLLER_ID,
	SERIAL_INPUT_CONTROLLER_ID,
	DATA_GENERATOR_CONTROLLER_ID,
	SERIAL_OUTPUT_CONTROLLER_ID,
	MCBSP_INPUT_CONTROLLER_ID,
	MCBSP_OUTPUT_CONTROLLER_ID,
	ERROR_CHECK_SOURCE_CONTROLLER_ID,
	ERROR_CHECK_DESTINATION_CONTROLLER_ID,
	PACKET_CREATOR_CONTROLLER_ID,
	PACKET_PARSER_CONTROLLER_ID,	
	TX_GATE_CONTROLLER_ID,
	RX_GATE_CONTROLLER_ID,
	AWGN_CONTROLLER_ID,
	WORD_TO_BYTE_CONTROLLER_ID,
	BYTE_TO_WORD_CONTROLLER_ID,
	NUM_OF_DATA_CONTROLLERS
} DATA_CONTROLLERS_LIST_FOR_HW;
	

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************


#define	SIZEOF_MCBSP_OUTPUT_BUFFER		        128
#define	SIZEOF_SERIAL_OUTPUT_DISPLAY_BUFFER		32


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern 		DATA_CONTROLLER	SerialInputController;
extern 		DATA_CONTROLLER	DataGeneratorController;
extern 		DATA_CONTROLLER	SerialOutputController;
extern      	DATA_CONTROLLER 	McBspOutputController;
extern      	DATA_CONTROLLER 	McBspInputController;
extern		DATA_CONTROLLER 	VEncoderController;
extern		DATA_CONTROLLER 	VDecoderController;
extern		DATA_CONTROLLER	TestModulatorController;
extern		DATA_CONTROLLER	ErrorCheckSourceController;
extern		DATA_CONTROLLER	ErrorCheckDestinationController;
extern		DATA_CONTROLLER	AWGNChannelController;
extern 		DATA_CONTROLLER	PacketCreaterController;
extern 		DATA_CONTROLLER	PacketParserController;
extern      	DATA_CONTROLLER 	FSKModulatorController;
extern      	DATA_CONTROLLER 	FSKDemodulatorController;
extern      DATA_CONTROLLER BurstTxController;
extern      DATA_CONTROLLER BurstRxController;
extern 		DATA_CONTROLLER	WordToByteController;
extern 		DATA_CONTROLLER	ByteToWordController;
extern		DATA_CONTROLLER	TxGateController;
extern		DATA_CONTROLLER	RxGateController;
extern		DATA_CONTROLLER	ChatController;
extern		DATA_CONTROLLER 	DataCheckController;

extern 		ROM DATA_CONTROLLER_INIT_STRUCT DataControllersInstantiationList[];

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************




#endif
