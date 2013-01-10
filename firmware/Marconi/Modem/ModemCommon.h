#ifndef __MODEMHEADERS_H__
	#define __MODEMHEADERS_H__


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




//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S
//******************************************************************************
typedef enum
{
	MODEM_A,
	MODEM_B,
	NUM_OF_MODEM_ID
} MODEM_ID;

//******************************************************************************
//  I N C L U D E S
//******************************************************************************
//#define PLATFORM_SIM

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>


#include "Config.h"
#include "Rescources.h"


#include "ModemInit.h"


#include "ObjStructs.h"
#include "InitObjects.h"
#include "Pipelines.h"
#include "DataController.h"
#include "Pipeline.h"

#include "ViterbiDefs.h"
#include "ViterbiEncoder.h"
#include "ViterbiDecoder.h"
#include "TxRxGates.h"
#include "Modulator.h"
#include "ErrorCheck.h"
#include "AWGN.h"
#include "FSKModulator.h"
#include "FSKDemodulator.h"
#include "BurstTx.h"
#include "BurstRx.h"
#include "DataGenerator.h"
#include "WordToByte.h"
#include "DataCheck.h"
#include "ByteToWord.h"

#ifdef PLATFORM_HW
#include "SerialOutput.h"
#include "SerialInput.h"
#include "McBspOutput.h"
#include "McBspInput.h"
#include "AppChat.h"

#elif PLATFORM_SIM

#include "FileReader.h"
#include "FileWriter.h"
#include "FileIOConfig.h"
#include "RadioSim.h"
#include "TestDecoder.h"
#include "PassThrough.h"

#endif



#include "PacketFormat.h"
#include "PacketCreater.h"
#include "PacketParser.h"





#include "Controllers.h"

#include "ModemInit.h"


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************




#endif
