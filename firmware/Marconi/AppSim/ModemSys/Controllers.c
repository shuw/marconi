//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		This file instantiates all controller blocks
//
//
//
//******************************************************************************


#define __CONTROLLERS_C__


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

//****************************************
// Data Generator Controller Variables and Structures
//****************************************
DATA_CONTROLLER DataGeneratorController;
DATA_GENERATOR_OBJ_STRUCT DataGeneratorObjStruct;

//****************************************
// Data Check Controller Variables and Structures
//****************************************
DATA_CONTROLLER DataCheckController;
DATA_CHECK_OBJ_STRUCT DataCheckObjStruct;

//****************************************
// File Controller Variables and Structures
//****************************************
DATA_CONTROLLER				FileReaderControllerA;
FILE_READER_OBJ_STRUCT			FileReaderObjStructA;
ROM FILE_READER_INIT_STRUCT		FileReaderInitStructA = {(BYTE *)INPUT_FILE_NAME, INPUT_FILE_TYPE};

DATA_CONTROLLER				FileWriterControllerA;
FILE_WRITER_OBJ_STRUCT			FileWriterObjStructA;
ROM FILE_WRITER_INIT_STRUCT		FileWriterInitStructA = {(BYTE *)OUTPUTA_FILE_NAME, OUTPUT_FILE_TYPE};


DATA_CONTROLLER				FileWriterControllerB;
FILE_WRITER_OBJ_STRUCT			FileWriterObjStructB;
ROM FILE_WRITER_INIT_STRUCT		FileWriterInitStructB = {(BYTE *)OUTPUTB_FILE_NAME, OUTPUT_FILE_TYPE};


//****************************************
// Viterbi Encoder Variables and Structures
//****************************************
VITERBI_ENCODER_OBJ_STRUCT		VEncoderObjStructA;
DATA_CONTROLLER				VEncoderControllerA;

VITERBI_ENCODER_OBJ_STRUCT		VEncoderObjStructB;
DATA_CONTROLLER				VEncoderControllerB;


//****************************************
// Viterbi TestDecoder Variables and Structures
//****************************************
VITERBI_DECODER_OBJ_STRUCT	VDecoderObjStructA;
DATA_CONTROLLER				VDecoderControllerA;

VITERBI_DECODER_OBJ_STRUCT	VDecoderObjStructB;
DATA_CONTROLLER				VDecoderControllerB;


//****************************************
// AWGN Channel Variables and Structures	
//****************************************
AWGN_CHANNEL_INIT_STRUCT	AWGNChannelInitStructA = 
	{
		DEFAULT_SNR_DB,
	};
AWGN_CHANNEL_OBJ_STRUCT	AWGNChannelObjStructA;
DATA_CONTROLLER			AWGNChannelControllerAtoB;

AWGN_CHANNEL_INIT_STRUCT	AWGNChannelInitStructB =
	{
		DEFAULT_SNR_DB,
	};
AWGN_CHANNEL_OBJ_STRUCT	AWGNChannelObjStructB;
DATA_CONTROLLER			AWGNChannelControllerBtoA;

//****************************************
// PacketCreater Variables and Structures	
// PacketParser Variables and Structures	
//****************************************
PACKET_SHARED_OBJ_STRUCT 		PacketSharedObjStructA;
PACKET_SHARED_OBJ_STRUCT 		PacketSharedObjStructB;
ROM PACKET_SHARED_INIT_STRUCT	PacketSharedInitStructA =
	{
		FALSE,
	};
ROM PACKET_SHARED_INIT_STRUCT	PacketSharedInitStructB =
	{
		FALSE,
	};
DATA_CONTROLLER				PacketCreaterControllerA;
DATA_CONTROLLER				PacketCreaterControllerB;
DATA_CONTROLLER				PacketParserControllerA;
DATA_CONTROLLER				PacketParserControllerB;

//****************************************
// TestModulator Variables and Structures
//****************************************
DATA_CONTROLLER				TestModulatorControllerA;

DATA_CONTROLLER				TestModulatorControllerB;



//****************************************
// Error Checker Variables and Structures	
//****************************************
ERROR_CHECK_OBJ_STRUCT		ErrorCheckObjStructA;
ROM ERROR_CHECK_INIT_STRUCT	ErrorCheckInitStructA = 
	{
		&VEncoderObjStructA.totalBitsCoded,				//	DWORD *totalBitsCoded
		ZERO_PTR,									//	DWORD *totalBitsTransmitted
		&VDecoderObjStructB.accumPathError,			//	DWORD	*viterbiPathError;
		(WORD *)ZERO_PTR,				//	DWORD *syncDropped
		&VDecoderObjStructB.numOfBitDroppedDetected, 	//	WORD 	*bitShiftDetected;
		&PacketSharedObjStructA.CreaterStruct.packetsTransmitted,		//	WORD 	*packetsTransmitted;
		&PacketSharedObjStructB.ParserStruct.packetsReceived, 		//	WORD 	*packetsReceived;
		&AWGNChannelObjStructA.numOfDroppedBits,	//	WORD 	*bitsDroppedInChannel;
		ERROR_CHECK_FILE_PROGRESS_MONITOR_FLAG	//	WORD	optionsFlags
		| ERROR_CHECK_LIVE_COMPARISON_FLAG
	};
DATA_CONTROLLER	ErrorCheckSourceControllerA;
DATA_CONTROLLER	ErrorCheckDestinationControllerA;

ERROR_CHECK_OBJ_STRUCT		ErrorCheckObjStructB;
ROM ERROR_CHECK_INIT_STRUCT	ErrorCheckInitStructB = 
	{
		&VEncoderObjStructB.totalBitsCoded,				//	DWORD *totalBitsCoded
		ZERO_PTR,									//	DWORD *totalBitsTransmitted
		&VDecoderObjStructA.accumPathError,			//	DWORD	*viterbiPathError;
		(WORD *)ZERO_PTR,				//	DWORD *syncDropped
		&VDecoderObjStructA.numOfBitDroppedDetected, 	//	WORD 	*bitShiftDetected;
		&PacketSharedObjStructB.CreaterStruct.packetsTransmitted,		//	WORD 	*packetsTransmitted;
		&PacketSharedObjStructA.ParserStruct.packetsReceived, 		//	WORD 	*packetsReceived;
		&AWGNChannelObjStructB.numOfDroppedBits,	//	WORD 	*bitsDroppedInChannel;
		ERROR_CHECK_FILE_PROGRESS_MONITOR_FLAG	//	WORD	optionsFlags
		| ERROR_CHECK_LIVE_COMPARISON_FLAG
	};
DATA_CONTROLLER	ErrorCheckSourceControllerB;
DATA_CONTROLLER	ErrorCheckDestinationControllerB;

//****************************************
// Tx Rx Gates Variables and Structures	
//****************************************
DATA_CONTROLLER			TxGateControllerA;
DATA_CONTROLLER			RxGateControllerA;
TXRX_GATES_INIT_STRUCT		TxRxGatesInitStructA = 
	{
		MODEM_A
	};
TXRX_GATES_OBJ_STRUCT		TxRxGatesObjStructA;


DATA_CONTROLLER			TxGateControllerB;
DATA_CONTROLLER			RxGateControllerB;
TXRX_GATES_INIT_STRUCT		TxRxGatesInitStructB = 
	{
		MODEM_B
	};
TXRX_GATES_OBJ_STRUCT		TxRxGatesObjStructB;


//****************************************
// Radio Simulator Variables and Structures	
//****************************************
RADIO_SYSTEM			TestRadioSystem;

DATA_CONTROLLER 		RadioSimControllerAtoB;
RADIO_SIM_OBJ_STRUCT	RadioSimObjStructAtoB;
RADIO_INIT_OBJ_STRUCT	RadioSimInitStructAtoB =
	{
		MODEM_A,
		&TestRadioSystem
	};
	
DATA_CONTROLLER 		RadioSimControllerBtoA;
RADIO_SIM_OBJ_STRUCT	RadioSimObjStructBtoA;
RADIO_INIT_OBJ_STRUCT	RadioSimInitStructBtoA =
	{
		MODEM_B,
		&TestRadioSystem
	};


//****************************************
// FSKModulator Variables and Structures
//****************************************
DATA_CONTROLLER             FSKModulatorControllerA;
FSK_MODULATOR_STATE_TYPE    FSKModulatorStateA;


//****************************************
// FSKDemodulator Variables and Structures
//****************************************
DATA_CONTROLLER             FSKDemodulatorControllerA;
FSK_DEMODULATOR_STATE_TYPE  FSKDemodulatorStateA;

//****************************************
// FSKModulator Variables and Structures
//****************************************
DATA_CONTROLLER             FSKModulatorControllerB;
FSK_MODULATOR_STATE_TYPE    FSKModulatorStateB;


//****************************************
// FSKDemodulator Variables and Structures
//****************************************
DATA_CONTROLLER             FSKDemodulatorControllerB;
FSK_DEMODULATOR_STATE_TYPE  FSKDemodulatorStateB;


//****************************************
// BurstTx and BurstRx 
//****************************************
DATA_CONTROLLER             BurstTxControllerA;
DATA_CONTROLLER             BurstTxControllerB;
DATA_CONTROLLER             BurstRxControllerA;
DATA_CONTROLLER             BurstRxControllerB;

BURST_TX_STATE_TYPE         BurstTxStateA;
BURST_TX_STATE_TYPE         BurstTxStateB;
BURST_RX_STATE_TYPE         BurstRxStateA;
BURST_RX_STATE_TYPE         BurstRxStateB;

//****************************************
// WordToByte Variables and Structures
//****************************************
DATA_CONTROLLER		WordToByteControllerA;
DATA_CONTROLLER		WordToByteControllerB;
//****************************************
// TestDecoderController Variables and Structures
//****************************************
DATA_CONTROLLER 		TestDecoderControllerA;
DATA_CONTROLLER 		TestDecoderControllerB;


//****************************************
// Pass Through Blocks
//****************************************
DATA_CONTROLLER 	PassThruAtoRadioSimController;	
DATA_CONTROLLER 	PassThruRadioSimToBController;

DATA_CONTROLLER 	PassThruBtoRadioSimController;	
DATA_CONTROLLER 	PassThruRadioSimToAController;


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
WORD			fileWriterBufferA[SIZEOF_FILE_WRITER_BUFFER];
WORD			VEncoderBufferA[SIZEOF_VENCODER_BUFFER];
WORD			VDecoderBufferA[SIZEOF_VDECODER_BUFFER];
WORD			testModulatorBufferA[SIZEOF_TESTMODULATOR_BUFFER];
WORD			AWGNChannelBufferA[SIZEOF_AWGNCHANNEL_BUFFER];
WORD			errorCheckSourceBufferA[SIZEOF_EC_SOURCE_BUFFER];
WORD			errorCheckDestinationBufferA[SIZEOF_EC_DESTINATION_BUFFER];
WORD			packetCreaterBufferA[SIZEOF_PACKET_CREATER_BUFFER];
WORD			packetParserBufferA[SIZEOF_PACKET_PARSER_BUFFER];
WORD			txGateBufferA[SIZEOF_TXGATE_BUFFER];
WORD			rxGateBufferA[SIZEOF_RXGATE_BUFFER];
	
WORD			fileWriterBufferB[SIZEOF_FILE_WRITER_BUFFER];
WORD			VEncoderBufferB[SIZEOF_VENCODER_BUFFER];
WORD			VDecoderBufferB[SIZEOF_VDECODER_BUFFER];
WORD			testModulatorBufferB[SIZEOF_TESTMODULATOR_BUFFER];
WORD			AWGNChannelBufferB[SIZEOF_AWGNCHANNEL_BUFFER];
WORD			errorCheckSourceBufferB[SIZEOF_EC_SOURCE_BUFFER];
WORD			errorCheckDestinationBufferB[SIZEOF_EC_DESTINATION_BUFFER];
WORD			packetCreaterBufferB[SIZEOF_PACKET_CREATER_BUFFER];
WORD			packetParserBufferB[SIZEOF_PACKET_PARSER_BUFFER];
WORD			txGateBufferB[SIZEOF_TXGATE_BUFFER];
WORD			rxGateBufferB[SIZEOF_RXGATE_BUFFER];


WORD			radioSimBufferAtoB[SIZEOF_RADIO_SIM_BUFFER];
WORD			radioSimBufferBtoA[SIZEOF_RADIO_SIM_BUFFER];


WORD            fskModulatorBufferA[SIZEOF_FSK_MODULATOR_BUFFER];
WORD            fskDemodulatorBufferA[SIZEOF_FSK_DEMODULATOR_BUFFER];
WORD            fskModulatorBufferB[SIZEOF_FSK_MODULATOR_BUFFER];
WORD            fskDemodulatorBufferB[SIZEOF_FSK_DEMODULATOR_BUFFER];
WORD            burstTxBufferA[SIZEOF_BURST_TX_BUFFER];
WORD            burstTxBufferB[SIZEOF_BURST_TX_BUFFER];
WORD            burstRxBufferA[SIZEOF_BURST_RX_BUFFER];
WORD            burstRxBufferB[SIZEOF_BURST_RX_BUFFER];


WORD			wordToByteBufferA[SIZEOF_WORD_TO_BYTE_BUFFER];
WORD			wordToByteBufferB[SIZEOF_WORD_TO_BYTE_BUFFER];
WORD			testDecoderBufferA[SIZEOF_TEST_DECODER_BUFFER];
WORD			testDecoderBufferB[SIZEOF_TEST_DECODER_BUFFER];

WORD			passThruAtoRadioSimBuffer[SIZEOF_PASS_THRU_BUFFER];
WORD			passThruRadioSimToBBuffer[SIZEOF_PASS_THRU_BUFFER];

WORD			passThruBtoRadioSimBuffer[SIZEOF_PASS_THRU_BUFFER];
WORD			passThruRadioSimToABuffer[SIZEOF_PASS_THRU_BUFFER];

WORD			dataCheckBuffer[SIZEOF_DATACHECK_BUFFER];


//******************************************************************************
//  C O D E
//******************************************************************************

ROM DATA_CONTROLLER_INIT_STRUCT DataControllersInstantiationList[NUM_OF_DATA_CONTROLLERS] = 
{
	{
/* Controller Pointer	*/	&DataGeneratorController,	 
/* Name of controller	*/	"Data Generator",				
/* Block Function        	*/	&DataGenerator,				
/* Ready Function       	*/	&DataGeneratorReady,		
/* Init Function            	*/	&DataGeneratorInit,
/* Block Memory Struct	*/	(BYTE *)&DataGeneratorObjStruct,					
/* Block Init Struct      	*/	NULL,					
/* Input Buffer Pointer  	*/	NULL,					
/* Size of Input Buffer   */	0,
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
/* Controller Pointer	*/	&FileReaderControllerA,	 
/* Name of controller	*/	"File Reader A",				
/* Block Function        	*/	&FileReader,				
/* Ready Function       	*/	&FileReaderReady,		
/* Init Function            	*/	&FileReaderInit,
/* Block Memory Struct	*/	(BYTE *)&FileReaderObjStructA,
/* Block Init Struct      	*/	(BYTE*)&FileReaderInitStructA,					
/* Input Buffer Pointer  	*/	NULL,					
/* Size of Input Buffer  	*/	0,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&FileWriterControllerA,
/* Name of controller	*/	"File Writer A",
/* Block Function        	*/	&FileWriter,
/* Ready Function       	*/	&FileWriterReady,
/* Init Function            	*/	&FileWriterInit,
/* Block Memory Struct	*/	(BYTE*)&FileWriterObjStructA,
/* Block Init Struct      	*/	(BYTE*)&FileWriterInitStructA,
/* Input Buffer Pointer  	*/	fileWriterBufferA,
/* Size of Input Buffer  	*/	SIZEOF_FILE_WRITER_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&ErrorCheckSourceControllerA,
/* Name of controller	*/	"Error Check Source A",
/* Block Function        	*/	&ErrorCheckSource,
/* Ready Function       	*/	&ErrorCheckSourceReady,
/* Init Function            	*/	&ErrorCheckInit,
/* Block Memory Struct	*/	(BYTE*)&ErrorCheckObjStructA,
/* Block Init Struct      	*/	(BYTE*)&ErrorCheckInitStructA,
/* Input Buffer Pointer  	*/	errorCheckSourceBufferA,
/* Size of Input Buffer  	*/	SIZEOF_EC_SOURCE_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&ErrorCheckDestinationControllerA,
/* Name of controller	*/	"ECd A",
/* Block Function        	*/	&ErrorCheckDestination,
/* Ready Function       	*/	&ErrorCheckDestinationReady,
/* Init Function            	*/	&ErrorCheckInit,
/* Block Memory Struct	*/	(BYTE *)&ErrorCheckObjStructA,
/* Block Init Struct      	*/	(BYTE *)&ErrorCheckInitStructA,
/* Input Buffer Pointer  	*/	errorCheckDestinationBufferA,
/* Size of Input Buffer  	*/	SIZEOF_EC_DESTINATION_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&VEncoderControllerA,
/* Name of controller	*/	"Viterbi Encoder A",
/* Block Function        	*/	&VEncoder,
/* Ready Function       	*/	&VEncoderReady,
/* Init Function            	*/	&VEncoderInit,
/* Block Memory Struct	*/	(BYTE *)&VEncoderObjStructA,
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	VEncoderBufferA,
/* Size of Input Buffer  	*/	SIZEOF_VENCODER_BUFFER,
/* Options Flags            */	0		
	},
	{
/* Controller Pointer	*/	&VDecoderControllerA,
/* Name of controller	*/	"Viterbi Decoder A",
/* Block Function        	*/	&VDecoder,
/* Ready Function       	*/	&VDecoderReady,
/* Init Function            	*/	&VDecoderInit,
/* Block Memory Struct	*/	(BYTE *)&VDecoderObjStructA,
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	VDecoderBufferA,
/* Size of Input Buffer  	*/	SIZEOF_VDECODER_BUFFER,
/* Options Flags            */	0		
	},
	{
/* Controller Pointer	*/	&AWGNChannelControllerAtoB,
/* Name of controller	*/	"AWGN Channel A",
/* Block Function        	*/	&AWGNChannel,
/* Ready Function       	*/	&AWGNChannelReady,
/* Init Function            	*/	&AWGNChannelInit,
/* Block Memory Struct	*/	(BYTE *)&AWGNChannelObjStructA,
/* Block Init Struct      	*/	(BYTE *)&AWGNChannelInitStructA,
/* Input Buffer Pointer  	*/	AWGNChannelBufferA,
/* Size of Input Buffer  	*/	SIZEOF_AWGNCHANNEL_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&TestModulatorControllerA,
/* Name of controller	*/	"Test Modulator A",
/* Block Function        	*/	&Modulator,
/* Ready Function       	*/	&ModulatorReady,
/* Init Function            	*/	NULL,
/* Block Memory Struct	*/	NULL,		
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	testModulatorBufferA,
/* Size of Input Buffer  	*/	SIZEOF_TESTMODULATOR_BUFFER,
/* Options Flags            */	0
	},		
	{
/* Controller Pointer	*/	&PacketCreaterControllerA,
/* Name of controller	*/	"Packet Creater A",
/* Block Function        	*/	&PacketCreater,
/* Ready Function       	*/	&PacketCreaterReady,
/* Init Function            	*/	&PacketCreaterInit,
/* Block Memory Struct	*/	(BYTE *)&PacketSharedObjStructA,
/* Block Init Struct      	*/	(BYTE *)&PacketSharedInitStructA,
/* Input Buffer Pointer  	*/	packetCreaterBufferA,
/* Size of Input Buffer  	*/	SIZEOF_PACKET_CREATER_BUFFER,
/* Options Flags            */	CONTROLLER_OPTIONS_PRINT_DEBUG
	},
	{
/* Controller Pointer	*/	&PacketParserControllerA,
/* Name of controller	*/	"Packet Parser  A",
/* Block Function        	*/	&PacketParser,
/* Ready Function       	*/	&PacketParserReady,
/* Init Function            	*/	&PacketParserInit,
/* Block Memory Struct	*/	(BYTE *)&PacketSharedObjStructA,
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	packetParserBufferA,
/* Size of Input Buffer  	*/	SIZEOF_PACKET_PARSER_BUFFER,
/* Options Flags            */	CONTROLLER_OPTIONS_PRINT_DEBUG
	},
	{
/* Controller Pointer	*/	&TxGateControllerA,
/* Name of controller	*/	"Transmit Gate A",
/* Block Function        	*/	&TxGate,
/* Ready Function       	*/	&TxGateReady,
/* Init Function            	*/	&TxGateInit,
/* Block Memory Struct	*/	(BYTE *)&TxRxGatesObjStructA,
/* Block Init Struct      	*/	(BYTE *)&TxRxGatesInitStructA,
/* Input Buffer Pointer  	*/	txGateBufferA,
/* Size of Input Buffer  	*/	SIZEOF_TXGATE_BUFFER,
#ifdef DEBUG_TXGATE
/* Options Flags            */	CONTROLLER_OPTIONS_PRINT_DEBUG
#else
/* Options Flags            */	0
#endif
	},
	{
/* Controller Pointer	*/	&RxGateControllerA,
/* Name of controller	*/	"Receive Gate A",
/* Block Function        	*/	&RxGate,
/* Ready Function       	*/	&RxGateReady,
/* Init Function            	*/	&RxGateInit,
/* Block Memory Struct	*/	(BYTE *)&TxRxGatesObjStructA,
/* Block Init Struct      	*/	(BYTE *)&TxRxGatesInitStructA,
/* Input Buffer Pointer  	*/	rxGateBufferA,
/* Size of Input Buffer  	*/	SIZEOF_RXGATE_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&FileWriterControllerB,
/* Name of controller	*/	"File Writer B",
/* Block Function        	*/	&FileWriter,
/* Ready Function       	*/	&FileWriterReady,
/* Init Function            	*/	&FileWriterInit,
/* Block Memory Struct	*/	(BYTE*)&FileWriterObjStructB,
/* Block Init Struct      	*/	(BYTE*)&FileWriterInitStructB,
/* Input Buffer Pointer  	*/	fileWriterBufferB,
/* Size of Input Buffer  	*/	SIZEOF_FILE_WRITER_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&ErrorCheckSourceControllerB,
/* Name of controller	*/	"Error Check Source B",
/* Block Function        	*/	&ErrorCheckSource,
/* Ready Function       	*/	&ErrorCheckSourceReady,
/* Init Function            	*/	&ErrorCheckInit,
/* Block Memory Struct	*/	(BYTE*)&ErrorCheckObjStructB,
/* Block Init Struct      	*/	(BYTE*)&ErrorCheckInitStructB,
/* Input Buffer Pointer  	*/	errorCheckSourceBufferB,
/* Size of Input Buffer  	*/	SIZEOF_EC_SOURCE_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&ErrorCheckDestinationControllerB,
/* Name of controller	*/	"ECd B",
/* Block Function        	*/	&ErrorCheckDestination,
/* Ready Function       	*/	&ErrorCheckDestinationReady,
/* Init Function            	*/	&ErrorCheckInit,
/* Block Memory Struct	*/	(BYTE *)&ErrorCheckObjStructB,
/* Block Init Struct      	*/	(BYTE *)&ErrorCheckInitStructB,
/* Input Buffer Pointer  	*/	errorCheckDestinationBufferB,
/* Size of Input Buffer  	*/	SIZEOF_EC_DESTINATION_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&VEncoderControllerB,
/* Name of controller	*/	"Viterbi Encoder B",
/* Block Function        	*/	&VEncoder,
/* Ready Function       	*/	&VEncoderReady,
/* Init Function            	*/	&VEncoderInit,
/* Block Memory Struct	*/	(BYTE *)&VEncoderObjStructB,
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	VEncoderBufferB,
/* Size of Input Buffer  	*/	SIZEOF_VENCODER_BUFFER,
/* Options Flags            */	0		
	},
	{
/* Controller Pointer	*/	&VDecoderControllerB,
/* Name of controller	*/	"Viterbi Decoder B",
/* Block Function        	*/	&VDecoder,
/* Ready Function       	*/	&VDecoderReady,
/* Init Function            	*/	&VDecoderInit,
/* Block Memory Struct	*/	(BYTE *)&VDecoderObjStructB,
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	VDecoderBufferB,
/* Size of Input Buffer  	*/	SIZEOF_VDECODER_BUFFER,
/* Options Flags            */	0		
	},
	{
/* Controller Pointer	*/	&AWGNChannelControllerBtoA,
/* Name of controller	*/	"AWGN Channel B",
/* Block Function        	*/	&AWGNChannel,
/* Ready Function       	*/	&AWGNChannelReady,
/* Init Function            	*/	&AWGNChannelInit,
/* Block Memory Struct	*/	(BYTE *)&AWGNChannelObjStructB,
/* Block Init Struct      	*/	(BYTE *)&AWGNChannelInitStructB,
/* Input Buffer Pointer  	*/	AWGNChannelBufferB,
/* Size of Input Buffer  	*/	SIZEOF_AWGNCHANNEL_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&TestModulatorControllerB,
/* Name of controller	*/	"Test Modulator B",
/* Block Function        	*/	&Modulator,
/* Ready Function       	*/	&ModulatorReady,
/* Init Function            	*/	NULL,
/* Block Memory Struct	*/	NULL,		
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	testModulatorBufferB,
/* Size of Input Buffer  	*/	SIZEOF_TESTMODULATOR_BUFFER,
/* Options Flags            */	0
	},		
	{
/* Controller Pointer	*/	&PacketCreaterControllerB,
/* Name of controller	*/	"Packet Creater B",
/* Block Function        	*/	&PacketCreater,
/* Ready Function       	*/	&PacketCreaterReady,
/* Init Function            	*/	&PacketCreaterInit,
/* Block Memory Struct	*/	(BYTE *)&PacketSharedObjStructB,		
/* Block Init Struct      	*/	(BYTE *)&PacketSharedInitStructA,
/* Input Buffer Pointer  	*/	packetCreaterBufferB,
/* Size of Input Buffer  	*/	SIZEOF_PACKET_CREATER_BUFFER,
/* Options Flags            */	CONTROLLER_OPTIONS_PRINT_DEBUG
	},
	{
/* Controller Pointer	*/	&PacketParserControllerB,
/* Name of controller	*/	"Packet Parser  B",
/* Block Function        	*/	&PacketParser,
/* Ready Function       	*/	&PacketParserReady,
/* Init Function            	*/	&PacketParserInit,
/* Block Memory Struct	*/	(BYTE *)&PacketSharedObjStructB,
/* Block Init Struct      	*/	NULL,
/* Input Buffer Pointer  	*/	packetParserBufferB,
/* Size of Input Buffer  	*/	SIZEOF_PACKET_PARSER_BUFFER,
/* Options Flags            */	CONTROLLER_OPTIONS_PRINT_DEBUG
	},
	{
/* Controller Pointer	*/	&TxGateControllerB,
/* Name of controller	*/	"Transmit Gate B",
/* Block Function        	*/	&TxGate,
/* Ready Function       	*/	&TxGateReady,
/* Init Function            	*/	&TxGateInit,
/* Block Memory Struct	*/	(BYTE *)&TxRxGatesObjStructB,
/* Block Init Struct      	*/	(BYTE *)&TxRxGatesInitStructB,
/* Input Buffer Pointer  	*/	txGateBufferB,
/* Size of Input Buffer  	*/	SIZEOF_TXGATE_BUFFER,
#ifdef DEBUG_TXGATE
/* Options Flags            */	CONTROLLER_OPTIONS_PRINT_DEBUG
#else
/* Options Flags            */	0
#endif
	},
	{
/* Controller Pointer	*/	&RxGateControllerB,
/* Name of controller	*/	"Receive Gate B",
/* Block Function        	*/	&RxGate,
/* Ready Function       	*/	&RxGateReady,
/* Init Function            	*/	&RxGateInit,
/* Block Memory Struct	*/	(BYTE *)&TxRxGatesObjStructB,
/* Block Init Struct      	*/	(BYTE *)&TxRxGatesInitStructB,
/* Input Buffer Pointer  	*/	rxGateBufferB,
/* Size of Input Buffer  	*/	SIZEOF_RXGATE_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&RadioSimControllerAtoB,
/* Name of controller	*/	"Radio Simulator A to B",
/* Block Function        	*/	&RadioSim,
/* Ready Function       	*/	&RadioSimReady,
/* Init Function            	*/	&RadioSimInit,
/* Block Memory Struct	*/	(BYTE *)&RadioSimObjStructAtoB,
/* Block Init Struct      	*/	(BYTE *)&RadioSimInitStructAtoB,
/* Input Buffer Pointer  	*/	radioSimBufferAtoB,
/* Size of Input Buffer  	*/	SIZEOF_RADIO_SIM_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer	*/	&RadioSimControllerBtoA,
/* Name of controller	*/	"Radio Simulator B to A",
/* Block Function        	*/	&RadioSim,
/* Ready Function       	*/	&RadioSimReady,
/* Init Function            	*/	&RadioSimInit,
/* Block Memory Struct	*/	(BYTE *)&RadioSimObjStructBtoA,
/* Block Init Struct      	*/	(BYTE *)&RadioSimInitStructBtoA,
/* Input Buffer Pointer  	*/	radioSimBufferBtoA,
/* Size of Input Buffer  	*/	SIZEOF_RADIO_SIM_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  	&FSKModulatorControllerA,
/* Name of controller   */	"FSK Modulator A",
/* Block Function       */	&FSKModulator,
/* Ready Function       */	&FSKModulatorReady,
/* Init Function        */		&FSKModulatorInit,
/* Block Memory Struct	*/	(BYTE *)&FSKModulatorStateA,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	fskModulatorBufferA,
/* Size of Input Buffer */	SIZEOF_FSK_MODULATOR_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  	&FSKDemodulatorControllerA,
/* Name of controller   */	"FSK Demodulator A",
/* Block Function       */	&FSKDemodulator,
/* Ready Function       */	&FSKDemodulatorReady,
/* Init Function        */		&FSKDemodulatorInit,
/* Block Memory Struct	*/	(BYTE *)&FSKDemodulatorStateA,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	fskDemodulatorBufferA,
/* Size of Input Buffer */	SIZEOF_FSK_DEMODULATOR_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  	&FSKModulatorControllerB,
/* Name of controller   */	"FSK Modulator B",
/* Block Function       */	&FSKModulator,
/* Ready Function       */	&FSKModulatorReady,
/* Init Function        */		&FSKModulatorInit,
/* Block Memory Struct	*/	(BYTE *)&FSKModulatorStateB,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	fskModulatorBufferB,
/* Size of Input Buffer */	SIZEOF_FSK_MODULATOR_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  	&FSKDemodulatorControllerB,
/* Name of controller   */	"FSK Demodulator B",
/* Block Function       */	&FSKDemodulator,
/* Ready Function       */	&FSKDemodulatorReady,
/* Init Function        */		&FSKDemodulatorInit,
/* Block Memory Struct	*/	(BYTE *)&FSKDemodulatorStateB,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	fskDemodulatorBufferB,
/* Size of Input Buffer */	SIZEOF_FSK_DEMODULATOR_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  &BurstTxControllerA,
/* Name of controller   */	"BurstTx A",
/* Block Function       */	&BurstTx,
/* Ready Function       */	&BurstTxReady,
/* Init Function        */  &BurstTxInit,
/* Block Memory Struct	*/	(BYTE *)&BurstTxStateA,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	burstTxBufferA,
/* Size of Input Buffer */	SIZEOF_BURST_TX_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  &BurstRxControllerA,
/* Name of controller   */	"BurstRx A",
/* Block Function       */	&BurstRx,
/* Ready Function       */	&BurstRxReady,
/* Init Function        */  &BurstRxInit,
/* Block Memory Struct	*/	(BYTE *)&BurstRxStateA,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	burstRxBufferA,
/* Size of Input Buffer */	SIZEOF_BURST_RX_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  &BurstTxControllerB,
/* Name of controller   */	"BurstTx B",
/* Block Function       */	&BurstTx,
/* Ready Function       */	&BurstTxReady,
/* Init Function        */  &BurstTxInit,
/* Block Memory Struct	*/	(BYTE *)&BurstTxStateB,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	burstTxBufferB,
/* Size of Input Buffer */	SIZEOF_BURST_TX_BUFFER,
/* Options Flags            */	0
	},
    {
/* Controller Pointer   */  &BurstRxControllerB,
/* Name of controller   */	"BurstRx B",
/* Block Function       */	&BurstRx,
/* Ready Function       */	&BurstRxReady,
/* Init Function        */  &BurstRxInit,
/* Block Memory Struct	*/	(BYTE *)&BurstRxStateB,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	burstRxBufferB,
/* Size of Input Buffer */	SIZEOF_BURST_RX_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  	&WordToByteControllerA,
/* Name of controller   */	"Word To Byte Converter A",
/* Block Function       */	&WordToByte,
/* Ready Function       */	&WordToByteReady,
/* Init Function        */		&WordToByteInit,
/* Block Memory Struct	*/	NULL,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	wordToByteBufferA,
/* Size of Input Buffer */	SIZEOF_WORD_TO_BYTE_BUFFER,
/* Options Flags            */	0
	},
		{
/* Controller Pointer   */  	&WordToByteControllerB,
/* Name of controller   */	"Word To Byte Converter B",
/* Block Function       */	&WordToByte,
/* Ready Function       */	&WordToByteReady,
/* Init Function        */		&WordToByteInit,
/* Block Memory Struct	*/	NULL,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	wordToByteBufferB,
/* Size of Input Buffer */	SIZEOF_WORD_TO_BYTE_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  &TestDecoderControllerA,
/* Name of controller   */	"Test Decoder A",
/* Block Function       */	&TestDecoder,
/* Ready Function       */	&TestDecoderReady,
/* Init Function        */		NULL,
/* Block Memory Struct	*/	NULL,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	testDecoderBufferA,
/* Size of Input Buffer */	SIZEOF_TEST_DECODER_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  &TestDecoderControllerB,
/* Name of controller   */	"Test Decoder B",
/* Block Function       */	&TestDecoder,
/* Ready Function       */	&TestDecoderReady,
/* Init Function        */		NULL,
/* Block Memory Struct	*/	NULL,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	testDecoderBufferB,
/* Size of Input Buffer */	SIZEOF_TEST_DECODER_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  	&PassThruAtoRadioSimController,
/* Name of controller   */	"Pass A to Radio Sim",
/* Block Function       */	&PassThrough,
/* Ready Function       */	&PassThroughReady,
/* Init Function        */		NULL,
/* Block Memory Struct	*/	NULL,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	passThruAtoRadioSimBuffer,
/* Size of Input Buffer */	SIZEOF_PASS_THRU_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  	&PassThruRadioSimToBController,
/* Name of controller   */	"Pass Radio Sim to B",
/* Block Function       */	&PassThrough,
/* Ready Function       */	&PassThroughReady,
/* Init Function        */		NULL,
/* Block Memory Struct	*/	NULL,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	passThruRadioSimToBBuffer,
/* Size of Input Buffer */	SIZEOF_PASS_THRU_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  	&PassThruBtoRadioSimController,
/* Name of controller   */	"Pass B to Radio Sim",
/* Block Function       */	&PassThrough,
/* Ready Function       */	&PassThroughReady,
/* Init Function        */		NULL,
/* Block Memory Struct	*/	NULL,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	passThruBtoRadioSimBuffer,
/* Size of Input Buffer */	SIZEOF_PASS_THRU_BUFFER,
/* Options Flags            */	0
	},
	{
/* Controller Pointer   */  	&PassThruRadioSimToAController,
/* Name of controller   */	"Pass Radio Sim to A",
/* Block Function       */	&PassThrough,
/* Ready Function       */	&PassThroughReady,
/* Init Function        */		NULL,
/* Block Memory Struct	*/	NULL,
/* Block Init Struct    */	NULL,
/* Input Buffer Pointer */	passThruRadioSimToABuffer,
/* Size of Input Buffer */	SIZEOF_PASS_THRU_BUFFER,
/* Options Flags            */	0
	}

};




/***********************************  END  ************************************/
