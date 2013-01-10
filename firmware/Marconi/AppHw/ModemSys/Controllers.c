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


#define __CONTROLLER_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

//****************************************
// Data Check Controller Variables and Structures
//****************************************
DATA_CONTROLLER DataCheckController;
DATA_CHECK_OBJ_STRUCT DataCheckObjStruct;


//****************************************
// FSKModulator Variables and Structures
//****************************************
DATA_CONTROLLER             FSKModulatorController;
FSK_MODULATOR_STATE_TYPE    FSKModulatorState;

//****************************************
// FSKDemodulator Variables and Structures
//****************************************
DATA_CONTROLLER             FSKDemodulatorController;
FSK_DEMODULATOR_STATE_TYPE  FSKDemodulatorState;

//****************************************
// BurstTx and BurstRx 
//****************************************
DATA_CONTROLLER             BurstTxController;
DATA_CONTROLLER             BurstRxController;

BURST_TX_STATE_TYPE         BurstTxState;
BURST_RX_STATE_TYPE         BurstRxState;

//****************************************
// Viterbi Encoder Variables and Structures
//****************************************
VITERBI_ENCODER_OBJ_STRUCT	VEncoderObjStruct;
DATA_CONTROLLER		VEncoderController;

//****************************************
// Viterbi Decoder Variables and Structures
//****************************************
VITERBI_DECODER_OBJ_STRUCT	VDecoderObjStruct;
DATA_CONTROLLER		VDecoderController;

//****************************************
// Serial Input Controller
//****************************************
DATA_CONTROLLER SerialInputController;

//****************************************
// Data Generator Controller Variables and Structures
//****************************************
DATA_CONTROLLER DataGeneratorController;
DATA_GENERATOR_OBJ_STRUCT DataGeneratorObjStruct;


//****************************************
// Serial Output Controller Variables and Structures
//****************************************
DATA_CONTROLLER SerialOutputController;
SERIAL_OUTPUT_OBJ_STRUCT SerialOutputObjStruct;

//****************************************
// McBsp Input Controller Variables and Structures
//****************************************
DATA_CONTROLLER McBspInputController;

//****************************************
// McBsp Output Controller Variables and Structures
//****************************************
DATA_CONTROLLER McBspOutputController;


//****************************************
// Test Modulator Controller Variables and Structures
//****************************************
DATA_CONTROLLER	TestModulatorController;

//****************************************
// PacketCreater Variables and Structures	
// PacketParser Variables and Structures	
//****************************************
PACKET_SHARED_OBJ_STRUCT	PacketSharedObjStruct;
ROM PACKET_SHARED_INIT_STRUCT	PacketSharedInitStruct =
	{
		TRUE,
	};
DATA_CONTROLLER	PacketCreaterController;
DATA_CONTROLLER	PacketParserController;


//****************************************
// AWGN Channel Variables and Structures	
//****************************************
ROM AWGN_CHANNEL_INIT_STRUCT	AWGNChannelInitStruct = {DEFAULT_SNR_DB};
AWGN_CHANNEL_OBJ_STRUCT	AWGNChannelObjStruct;
DATA_CONTROLLER	AWGNChannelController;


//****************************************
// Error Checker Variables and Structures	
//****************************************
ERROR_CHECK_OBJ_STRUCT	ErrorCheckObjStruct;
ROM ERROR_CHECK_INIT_STRUCT	ErrorCheckInitStruct = 
	{
		&VEncoderObjStruct.totalBitsCoded,				//	DWORD *totalBitsCoded
		ZERO_PTR,									//	DWORD *totalBitsTransmitted
		&VDecoderObjStruct.accumPathError,			//	DWORD	*viterbiPathError;
		(WORD *)ZERO_PTR,				//	DWORD *syncDropped
		&VDecoderObjStruct.numOfBitDroppedDetected, 	//	WORD 	*bitShiftDetected;
		ZERO_PTR,		//	WORD 	*packetsTransmitted;
		ZERO_PTR, 		//	WORD 	*packetsReceived;
		&AWGNChannelObjStruct.numOfDroppedBits,	//	WORD 	*bitsDroppedInChannel;
		ERROR_CHECK_FILE_PROGRESS_MONITOR_FLAG	//	WORD	optionsFlags
		| ERROR_CHECK_LIVE_COMPARISON_FLAG
	};
DATA_CONTROLLER	ErrorCheckSourceController;
DATA_CONTROLLER	ErrorCheckDestinationController;

//****************************************
// Tx Rx Gates Variables and Structures	
//****************************************
DATA_CONTROLLER			TxGateController;
DATA_CONTROLLER			RxGateController;
TXRX_GATES_INIT_STRUCT		TxRxGatesInitStruct= 
	{
		MODEM_A
	};
TXRX_GATES_OBJ_STRUCT		TxRxGatesObjStruct;

//****************************************
// WordToByte Variables and Structures
//****************************************
DATA_CONTROLLER		WordToByteController;

//****************************************
// ByteToWord Variables and Structures
//****************************************
DATA_CONTROLLER		ByteToWordController;

//****************************************
// Chat Program
//****************************************
DATA_CONTROLLER		ChatController;
CHAT_OBJ_STRUCT		ChatObjStruct;

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
WORD			serialOutputBuffer[SIZEOF_SERIAL_OUTPUT_DISPLAY_BUFFER];
WORD			VEncoderBuffer[SIZEOF_VENCODER_BUFFER];
WORD			VDecoderBuffer[SIZEOF_VDECODER_BUFFER];
WORD			testModulatorBuffer[SIZEOF_TESTMODULATOR_BUFFER];
WORD			errorCheckSourceBuffer[SIZEOF_EC_SOURCE_BUFFER];
WORD			errorCheckDestinationBuffer[SIZEOF_EC_DESTINATION_BUFFER];
WORD			AWGNChannelBuffer[SIZEOF_AWGNCHANNEL_BUFFER];
WORD			packetCreaterBuffer[SIZEOF_PACKET_CREATER_BUFFER];
WORD			packetParserBuffer[SIZEOF_PACKET_PARSER_BUFFER];
WORD			mcBspOutputBuffer[SIZEOF_MCBSP_OUTPUT_BUFFER];
WORD            fskModulatorBuffer[SIZEOF_FSK_MODULATOR_BUFFER];
WORD            fskDemodulatorBuffer[SIZEOF_FSK_DEMODULATOR_BUFFER];
WORD            burstTxBuffer[SIZEOF_BURST_TX_BUFFER];
WORD            burstRxBuffer[SIZEOF_BURST_RX_BUFFER];
WORD            wordToByteBuffer[SIZEOF_WORD_TO_BYTE_BUFFER];
WORD            byteToWordBuffer[SIZEOF_BYTE_TO_WORD_BUFFER];
WORD            txGateBuffer[SIZEOF_TXGATE_BUFFER];
WORD			rxGateBuffer[SIZEOF_RXGATE_BUFFER];
WORD			dataCheckBuffer[SIZEOF_DATACHECK_BUFFER];

//******************************************************************************
//  C O D E
//******************************************************************************
ROM DATA_CONTROLLER_INIT_STRUCT DataControllersInstantiationList[NUM_OF_DATA_CONTROLLERS] = 
{
	{
/* Controller Pointer   */  		&ChatController,
/* Name of controller   */		"Chat",
/* Block Function       */		&AppChat,
/* Ready Function       */		&AppChatReady,
/* Init Function        */			&AppChatInit,
/* Block Memory Struct	*/		(BYTE *)&ChatObjStruct,
/* Block Init Struct    */		NULL,
/* Input Buffer Pointer */		NULL,
/* Size of Input Buffer */		0,
/* Options Flags            */		0,
	},
	{
/* Controller Pointer   */  &FSKModulatorController,
/* Name of controller   */	"FSK Modulator",
/* Block Function       */	&FSKModulator,
/* Ready Function       */	&FSKModulatorReady,
/* Init Function        */	&FSKModulatorInit,
/* Block Memory Struct	*/	(BYTE *)&FSKModulatorState,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	fskModulatorBuffer,
/* Size of Input Buffer */	SIZEOF_FSK_MODULATOR_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  	&FSKDemodulatorController,
/* Name of controller   */	"FSK Demodulator",
/* Block Function       */	&FSKDemodulator,
/* Ready Function       */	&FSKDemodulatorReady,
/* Init Function        */		&FSKDemodulatorInit,
/* Block Memory Struct	*/	(BYTE *)&FSKDemodulatorState,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	fskDemodulatorBuffer,
/* Size of Input Buffer */	SIZEOF_FSK_DEMODULATOR_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  &BurstTxController,
/* Name of controller   */	"BurstTx",
/* Block Function       */	&BurstTx,
/* Ready Function       */	&BurstTxReady,
/* Init Function        */  &BurstTxInit,
/* Block Memory Struct	*/	(BYTE *)&BurstTxState,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	burstTxBuffer,
/* Size of Input Buffer */	SIZEOF_BURST_TX_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  &BurstRxController,
/* Name of controller   */	"BurstRx",
/* Block Function       */	&BurstRx,
/* Ready Function       */	&BurstRxReady,
/* Init Function        */  &BurstRxInit,
/* Block Memory Struct	*/	(BYTE *)&BurstRxState,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	burstRxBuffer,
/* Size of Input Buffer */	SIZEOF_BURST_RX_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&VEncoderController,
/* Name of controller	*/	"Viterbi Encoder",
/* Block Function        	*/	&VEncoder,
/* Ready Function       	*/	&VEncoderReady,
/* Init Function            	*/	&VEncoderInit,
/* Block Memory Struct	*/	(BYTE *)&VEncoderObjStruct,		
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	VEncoderBuffer,
/* Size of Input Buffer  	*/	SIZEOF_VENCODER_BUFFER,
/* Options Flags            */	0		
	},
	{
/* Controller Pointer	*/	&VDecoderController,
/* Name of controller	*/	"Viterbi Decoder",
/* Block Function        	*/	&VDecoder,
/* Ready Function       	*/	&VDecoderReady,
/* Init Function            	*/	&VDecoderInit,
/* Block Memory Struct	*/	(BYTE *)&VDecoderObjStruct,		
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	VDecoderBuffer,
/* Size of Input Buffer  	*/	SIZEOF_VDECODER_BUFFER,
/* Options Flags            */	0		
	},
	{
/* Controller Pointer	*/	&TestModulatorController,
/* Name of controller	*/	"Test Modulator",
/* Block Function        	*/	&Modulator,
/* Ready Function       	*/	&ModulatorReady,
/* Init Function            	*/	NULL,
/* Block Memory Struct	*/	NULL,		
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	testModulatorBuffer,
/* Size of Input Buffer  	*/	SIZEOF_TESTMODULATOR_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&SerialInputController,
/* Name of controller	*/	"Serial Input",
/* Block Function        	*/	&SerialInput,
/* Ready Function       	*/	&SerialInputReady,
/* Init Function            	*/	&SerialInputInit,
/* Block Memory Struct	*/	NULL,		
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	NULL,
/* Size of Input Buffer  	*/	0,
/* Options Flags            */	0	
	},
	{
/* Controller Pointer	*/	&DataGeneratorController,	 
/* Name of controller	*/	"Data Generator",				
/* Block Function        	*/	&DataGenerator,				
/* Ready Function       	*/	&DataGeneratorReady,		
/* Init Function            	*/	&DataGeneratorInit,
/* Block Memory Struct	*/	(BYTE *)&DataGeneratorObjStruct,					
/* Block Init Struct      	*/	NULL,					
/* Input Buffer Pointer  	*/	NULL,					
/* Size of Input Buffer  	*/	0,
/* Options Flags            */	0						
	},
	{
/* Controller Pointer	*/	&DataCheckController,
/* Name of controller	*/	"Data Checker",				
/* Block Function        	*/	&DataCheck,				
/* Ready Function       	*/	&DataCheckReady,		
/* Init Function            	*/	&DataCheckInit,
/* Block Memory Struct	*/	(BYTE *)&DataCheckObjStruct,					
/* Block Init Struct      	*/	NULL,					
/* Input Buffer Pointer  	*/	dataCheckBuffer,					
/* Size of Input Buffer   */	SIZEOF_DATACHECK_BUFFER,
/* Options Flags            */	CONTROLLER_OPTIONS_PRINT_DEBUG
	},

	{
/* Controller Pointer	*/	&SerialOutputController,	 
/* Name of controller	*/	"Serial Output",				
/* Block Function        	*/	&SerialOutput,				
/* Ready Function       	*/	&SerialOutputReady,		
/* Init Function            	*/	&SerialOutputInit,
/* Block Memory Struct	*/	(BYTE *)&SerialOutputObjStruct,					
/* Block Init Struct      	*/	NULL,					
/* Input Buffer Pointer  	*/	serialOutputBuffer,					
/* Size of Input Buffer  	*/	SIZEOF_SERIAL_OUTPUT_DISPLAY_BUFFER,
/* Options Flags            */	0						
	},
	{
/* Controller Pointer	*/	&McBspInputController,	 
/* Name of controller	*/	"McBsp Input",				
/* Block Function        	*/	&McBspInput,				
/* Ready Function       	*/	&McBspInputReady,		
/* Init Function            	*/	&McBspInputInit,
/* Block Memory Struct	*/	NULL,					
/* Block Init Struct      	*/	NULL,					
/* Input Buffer Pointer  	*/	NULL,					
/* Size of Input Buffer  	*/	0,
/* Options Flags            */	0						
	},
	{
/* Controller Pointer	*/	&McBspOutputController,	 
/* Name of controller	*/	"McBsp Output",				
/* Block Function        	*/	&McBspOutput,
/* Ready Function       	*/	&McBspOutputReady,
/* Init Function            	*/	&McBspOutputInit,
/* Block Memory Struct	*/	NULL,					
/* Block Init Struct      	*/	NULL,					
/* Input Buffer Pointer  	*/	mcBspOutputBuffer,					
/* Size of Input Buffer  	*/	SIZEOF_MCBSP_OUTPUT_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&ErrorCheckSourceController,
/* Name of controller	*/	"Error Check Source",
/* Block Function        	*/	&ErrorCheckSource,
/* Ready Function       	*/	&ErrorCheckSourceReady,
/* Init Function            	*/	&ErrorCheckInit,
/* Block Memory Struct	*/	(BYTE*)&ErrorCheckObjStruct,		
/* Block Init Struct      	*/	(BYTE*)&ErrorCheckInitStruct,
/* Input Buffer Pointer  	*/	errorCheckSourceBuffer,
/* Size of Input Buffer  	*/	SIZEOF_EC_SOURCE_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&ErrorCheckDestinationController,
/* Name of controller	*/	"Error Check Destination",
/* Block Function        	*/	&ErrorCheckDestination,
/* Ready Function       	*/	&ErrorCheckDestinationReady,
/* Init Function            	*/	&ErrorCheckInit,
/* Block Memory Struct	*/	(BYTE *)&ErrorCheckObjStruct,		
/* Block Init Struct      	*/	(BYTE *)&ErrorCheckInitStruct,
/* Input Buffer Pointer  	*/	errorCheckDestinationBuffer,
/* Size of Input Buffer  	*/	SIZEOF_EC_DESTINATION_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&PacketCreaterController,
/* Name of controller	*/	"Packet Creater",
/* Block Function        	*/	&PacketCreater,
/* Ready Function       	*/	&PacketCreaterReady,
/* Init Function            	*/	&PacketCreaterInit,
/* Block Memory Struct	*/	(BYTE *)&PacketSharedObjStruct,		
/* Block Init Struct      	*/	(BYTE *)&PacketSharedInitStruct,
/* Input Buffer Pointer  	*/	packetCreaterBuffer,
/* Size of Input Buffer  	*/	SIZEOF_PACKET_CREATER_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&PacketParserController,
/* Name of controller	*/	"Packet Parser",
/* Block Function        	*/	&PacketParser,
/* Ready Function       	*/	&PacketParserReady,
/* Init Function            	*/	&PacketParserInit,
/* Block Memory Struct	*/	(BYTE *)&PacketSharedObjStruct,		
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	packetParserBuffer,
/* Size of Input Buffer  	*/	SIZEOF_PACKET_PARSER_BUFFER,
/* Options Flags            */	0
	},
    {
/* Controller Pointer	*/	&TxGateController,
/* Name of controller	*/	"Transmit Gate",
/* Block Function        	*/	&TxGate,
/* Ready Function       	*/	&TxGateReady,
/* Init Function            	*/	&TxGateInit,
/* Block Memory Struct	*/	(BYTE *)&TxRxGatesObjStruct,
/* Block Init Struct      	*/	(BYTE *)&TxRxGatesInitStruct,
/* Input Buffer Pointer  	*/	txGateBuffer,
/* Size of Input Buffer  	*/	SIZEOF_TXGATE_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&RxGateController,
/* Name of controller	*/	"Receive Gate",
/* Block Function        	*/	&RxGate,
/* Ready Function       	*/	&RxGateReady,
/* Init Function            	*/	&RxGateInit,
/* Block Memory Struct	*/	(BYTE *)&TxRxGatesObjStruct,
/* Block Init Struct      	*/	(BYTE *)&TxRxGatesInitStruct,
/* Input Buffer Pointer  	*/	rxGateBuffer,
/* Size of Input Buffer  	*/	SIZEOF_RXGATE_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&AWGNChannelController,
/* Name of controller	*/	"AWGN Channel",
/* Block Function        	*/	&AWGNChannel,
/* Ready Function       	*/	&AWGNChannelReady,
/* Init Function            	*/	&AWGNChannelInit,
/* Block Memory Struct	*/	(BYTE *)&AWGNChannelObjStruct,
/* Block Init Struct      	*/	(BYTE *)&AWGNChannelInitStruct,
/* Input Buffer Pointer  	*/	AWGNChannelBuffer,
/* Size of Input Buffer  	*/	SIZEOF_AWGNCHANNEL_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  &WordToByteController,
/* Name of controller   */	"Word To Byte Converter",
/* Block Function       */	&WordToByte,
/* Ready Function       */	&WordToByteReady,
/* Init Function        */	&WordToByteInit,
/* Block Memory Struct	*/	NULL,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	wordToByteBuffer,
/* Size of Input Buffer */	SIZEOF_WORD_TO_BYTE_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  &ByteToWordController,
/* Name of controller   */	"Byte To Word Converter",
/* Block Function       */	&ByteToWord,
/* Ready Function       */	&ByteToWordReady,
/* Init Function        */	&ByteToWordInit,
/* Block Memory Struct	*/	NULL,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	byteToWordBuffer,
/* Size of Input Buffer */	SIZEOF_BYTE_TO_WORD_BUFFER,
/* Options Flags            */	0
    }
};


/***********************************  END  ************************************/
