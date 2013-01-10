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


#ifndef __PIPELINES_H__
	#define __PIPELINES_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************


//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef enum
{
	MODEMA_TX_PIPELINE,
	MODEMB_TX_PIPELINE,
	MODEMA_RX_PIPELINE,
	MODEMB_RX_PIPELINE,
	RADIOSIM_A_TO_B_PIPELINE,
	RADIOSIM_B_TO_A_PIPELINE,
	VITERBI_MODULATOR_PIPELINE,
	DEMODULATOR_VITERBI_PIPELINE,
	MODULATOR_PIPELINE,
	DEMODULATOR_PIPELINE,
    BURST_TX_PIPELINE,
    BURST_RX_PIPELINE,
    TEST_TX_PIPELINE,
    TEST_RX_PIPELINE,
	NUM_OF_PIPELINES
} PIPELINES_LIST;




//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern PIPELINE ModemATxPipeline;
extern PIPELINE ModemBTxPipeline;
extern PIPELINE ModemARxPipeline;
extern PIPELINE ModemBRxPipeline;
extern PIPELINE RadioSimAtoBPipeline;
extern PIPELINE RadioSimBtoAPipeline;

extern PIPELINE ViterbiModulatorPipeline;
extern PIPELINE DemodulatorViterbiPipeline;
extern PIPELINE ModulatorPipeline;
extern PIPELINE DemodulatorPipeline;
extern PIPELINE BurstTxPipeline;
extern PIPELINE BurstRxPipeline;

extern PIPELINE TestTxPipeline;
extern PIPELINE TestRxPipeline;

extern ROM PIPELINE_INIT_STRUCT PipelinesInstantiationList[];


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************




#endif
