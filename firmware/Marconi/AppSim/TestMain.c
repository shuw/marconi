#include <stdio.h>
#include <string.h>
#include "All.h"


const char *TEST_PIPELINE_NAMES[] = {
    "ViterbiModulator",
    "DemodulatorViterbi",
    "Modulator",
    "Demodulator",
    "BurstTx",
    "BurstRx",
    "TestTx",
    "TestRx",
    0
};

PIPELINE *TEST_PIPELINES[] = {
    &ViterbiModulatorPipeline,
    &DemodulatorViterbiPipeline,
    &ModulatorPipeline,
    &DemodulatorPipeline,
    &BurstTxPipeline,
    &BurstRxPipeline,
    &TestTxPipeline,
    &TestRxPipeline,
    0
};


int main(int argc, char *argv[])
{
    FILE *fIn;
    char pipelineName[32];
    PIPELINE *pipeline = 0;
    WORD i;
    WORD idleCount;

    fIn = fopen("test_options.dat", "rb");
    fread(pipelineName, 32, 1, fIn);
    fclose(fIn);
    pipelineName[31] = 0;

	debugFileCaptureStream = fopen(DEBUG_OUT_FILE_NAME , "w+");

    for(i = 0; TEST_PIPELINE_NAMES[i] != 0 && TEST_PIPELINES[i] != 0; ++i) {
        if(strcmp(pipelineName, TEST_PIPELINE_NAMES[i]) == 0) {
            pipeline = TEST_PIPELINES[i];
            break;
        }
    }

    if(pipeline == 0) {
        printf("Error: invalid pipeline `%s`\n", pipelineName);
        return -1;
    }

    ModemInit();
    idleCount = 0;
    while(idleCount < 10) {
        if(PipelineFire(pipeline)) {
            idleCount = 0;
        }
        else {
            idleCount++;
        }
    }

    return 0;
}
