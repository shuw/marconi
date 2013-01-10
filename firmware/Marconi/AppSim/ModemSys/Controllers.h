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
	DATA_GENERATOR_CONTROLLER_ID,
	DATA_CHECK_CONTROLLER_ID,
	A_FILE_READER_CONTROLLER_ID,
	A_FILE_WRITER_CONTROLLER_ID,
	A_ERROR_CHECK_SOURCE_CONTROLLER_ID,
	A_ERROR_CHECK_DESTINATION_CONTROLLER_ID,
	A_VITERBI_DECODER_CONTROLLER_ID,
	A_VITERBI_ENCODER_CONTROLLER_ID,
	A_AWGN_CHANNEL_CONTROLLER_ID,
	A_TEST_MODULATOR_CONTROLLER_ID,
	A_PACKET_CREATER_CONTROLLER_ID,
	A_PACKET_PARSER_CONTROLLER_ID,
	A_TX_GATE_CONTROLLER_ID,
	A_RX_GATE_CONTROLLER_ID,
	B_FILE_WRITER_CONTROLLER_ID,
	B_ERROR_CHECK_SOURCE_CONTROLLER_ID,
	B_ERROR_CHECK_DESTINATION_CONTROLLER_ID,
	B_VITERBI_DECODER_CONTROLLER_ID,
	B_VITERBI_ENCODER_CONTROLLER_ID,
	B_AWGN_CHANNEL_CONTROLLER_ID,
	B_TEST_MODULATOR_CONTROLLER_ID,
	B_PACKET_CREATER_CONTROLLER_ID,
	B_PACKET_PARSER_CONTROLLER_ID,
	B_TX_GATE_CONTROLLER_ID,
	B_RX_GATE_CONTROLLER_ID,
	A_TO_B_RADIO_SIM_CONTROLLER_ID,
	B_TO_A_RADIO_SIM_CONTROLLER_ID,
	A_FSK_MODULATOR_CONTROLLER_ID,
	A_FSK_DEMODULATOR_CONTROLLER_ID,
	B_FSK_MODULATOR_CONTROLLER_ID,
	B_FSK_DEMODULATOR_CONTROLLER_ID,
    A_BURST_TX_CONTROLLER_ID,
    A_BURST_RX_CONTROLLER_ID,
    B_BURST_TX_CONTROLLER_ID,
    B_BURST_RX_CONTROLLER_ID,
	A_WORD_TO_BYTE_CONTROLLER_ID,
	B_WORD_TO_BYTE_CONTROLLER_ID,
	A_TEST_DECODER_CONTROLLER_ID,
	B_TEST_DECODER_CONTROLLER_ID,
	PASSTHRU_A_TO_RADIOSIM_CONTROLLER_ID,
	PASSTHRU_RADIOSIM_TO_B_CONTROLLER_ID,
	PASSTHRU_B_TO_RADIOSIM_CONTROLLER_ID,
	PASSTHRU_RADIOSIM_TO_A_CONTROLLER_ID,
	NUM_OF_DATA_CONTROLLERS
} DATA_CONTROLLERS_LIST;
	



//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define	SIZEOF_NOISEMEDIUMA_BUFFER      4
#define	SIZEOF_FILE_WRITER_BUFFER       128	
#define SIZEOF_RADIO_SIM_BUFFER         256
#define SIZEOF_TEST_DECODER_BUFFER      256
#define SIZEOF_PASS_THRU_BUFFER      	256

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern 		DATA_CONTROLLER	DataGeneratorController;
extern 		DATA_CONTROLLER	DataCheckController;

extern 		DATA_CONTROLLER	FileReaderControllerA;
extern 		DATA_CONTROLLER	FileWriterControllerA;
extern 		DATA_CONTROLLER	VEncoderControllerA;
extern 		DATA_CONTROLLER	VDecoderControllerA;
extern 		DATA_CONTROLLER	TestModulatorControllerA;
extern		DATA_CONTROLLER	AWGNChannelControllerAtoB;
extern		DATA_CONTROLLER	ErrorCheckSourceControllerA;
extern		DATA_CONTROLLER	ErrorCheckDestinationControllerA;
extern 		DATA_CONTROLLER	PacketCreaterControllerA;
extern 		DATA_CONTROLLER	PacketParserControllerA;
extern		DATA_CONTROLLER	TxGateControllerA;
extern		DATA_CONTROLLER	RxGateControllerA;
extern 		DATA_CONTROLLER	FileWriterControllerB;
extern 		DATA_CONTROLLER	VEncoderControllerB;
extern 		DATA_CONTROLLER	VDecoderControllerB;
extern 		DATA_CONTROLLER	TestModulatorControllerB;
extern		DATA_CONTROLLER	AWGNChannelControllerBtoA;
extern		DATA_CONTROLLER	ErrorCheckSourceControllerB;
extern		DATA_CONTROLLER	ErrorCheckDestinationControllerB;
extern 		DATA_CONTROLLER	PacketCreaterControllerB;
extern 		DATA_CONTROLLER	PacketParserControllerB;
extern		DATA_CONTROLLER	TxGateControllerB;
extern		DATA_CONTROLLER	RxGateControllerB;

extern 		DATA_CONTROLLER	RadioSimControllerAtoB;
extern 		DATA_CONTROLLER	RadioSimControllerBtoA;

extern      DATA_CONTROLLER FSKModulatorControllerA;
extern      DATA_CONTROLLER FSKDemodulatorControllerA;
extern      DATA_CONTROLLER FSKModulatorControllerB;
extern      DATA_CONTROLLER FSKDemodulatorControllerB;

extern      DATA_CONTROLLER BurstTxControllerA;
extern      DATA_CONTROLLER BurstTxControllerB;
extern      DATA_CONTROLLER BurstRxControllerA;
extern      DATA_CONTROLLER BurstRxControllerB;

extern 		DATA_CONTROLLER	WordToByteControllerA;
extern 		DATA_CONTROLLER	WordToByteControllerB;

extern      DATA_CONTROLLER TestDecoderControllerA;
extern      DATA_CONTROLLER TestDecoderControllerB;

extern      DATA_CONTROLLER PassThruAtoRadioSimController;	
extern      DATA_CONTROLLER PassThruRadioSimToBController;

extern      DATA_CONTROLLER PassThruBtoRadioSimController;	
extern      DATA_CONTROLLER PassThruRadioSimToAController;

extern 		ROM DATA_CONTROLLER_INIT_STRUCT DataControllersInstantiationList[];


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************



#endif
