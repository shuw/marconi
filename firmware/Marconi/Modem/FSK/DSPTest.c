// Test driver for DSP routines 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DSP.h"
#include "TargetFile.h"


static void TestIirBiquad(FILE *fIn, FILE *fOut);
static void TestIntegrateAndDump(FILE *fIn, FILE *fOut);
static void TestSqrt(FILE *fIn, FILE *fOut);
static void TestSignificantBits(FILE *fIn, FILE *fOut);


int main(int argc, char *argv[]) 
{
    FILE *fIn;
    FILE *fOut;
    char option[32];
    char inputFile[32];
    char outputFile[32];

    fIn = fopen("test_options.dat", "rb");
    fread(option, sizeof(option), 1, fIn);
    fread(inputFile, sizeof(inputFile), 1, fIn);
    fread(outputFile, sizeof(outputFile), 1, fIn);
    fclose(fIn);

    fIn = fopen(inputFile, "rb");
    fOut = fopen(outputFile, "wb");
    
    if(fIn && fOut) {
        if(strcmp(option, "IirBiquad") == 0) {
            TestIirBiquad(fIn, fOut);
        }
        else if(strcmp(option, "IntegrateAndDump") == 0) {
            TestIntegrateAndDump(fIn, fOut);
        }
        else if(strcmp(option, "Sqrt") == 0) {
            TestSqrt(fIn, fOut);
        }
        else if(strcmp(option, "SignificantBits") == 0) {
            TestSignificantBits(fIn, fOut);
        }
        else {
            printf("Error: invalid test case '%s'!\n", option);
        }
    }

    if(fIn) fclose(fIn);
    if(fOut) fclose(fOut);

    return 0;
}


static void TestIirBiquad(FILE *fIn, FILE *fOut)
{
    IIR_BIQUAD_TYPE b;
    WORD count;
    SWORD in, out;

    TargetFileRead(&b.shift, sizeof(b.shift), 1, fIn);
    TargetFileRead(&b.b1, sizeof(b.b1), 1, fIn);
    TargetFileRead(&b.b2, sizeof(b.b2), 1, fIn);
    TargetFileRead(&b.b3, sizeof(b.b3), 1, fIn);
    TargetFileRead(&b.a2, sizeof(b.a2), 1, fIn);
    TargetFileRead(&b.a3, sizeof(b.a3), 1, fIn);
    b.dx1 = 0;
    b.dx2 = 0;
    b.dy1 = 0;
    b.dy2 = 0;

    TargetFileRead(&count, sizeof(WORD), 1, fIn);

    while(count-- > 0) {
        TargetFileRead(&in, sizeof(SWORD), 1, fIn);
        out = IirBiquad(&b, in);
        TargetFileWrite(&out, sizeof(SWORD), 1, fOut);
    }
}


static void TestIntegrateAndDump(FILE *fIn, FILE *fOut)
{
    WORD length;
    WORD shift;
    SWORD factor;
    SWORD in[128];
    SWORD out;

    TargetFileRead(&length, sizeof(length), 1, fIn);
    TargetFileRead(&shift, sizeof(shift), 1, fIn);
    TargetFileRead(&factor, sizeof(factor), 1, fIn);
    if(length > sizeof(in) / sizeof(in[0])) {
        printf("Error: buffer too small in TestIntegrateAndDump()\n");
        return;
    }
    TargetFileRead(in, sizeof(in[0]), length, fIn);

    out = IntegrateAndDump(in, length, shift, factor);

    TargetFileWrite(&out, sizeof(out), 1, fOut);
}


static void TestSqrt(FILE *fIn, FILE *fOut)
{
    DWORD count;
    DWORD x;
    WORD y;

    TargetFileRead(&count, sizeof(count), 1, fIn);

    while(count-- > 0) {
        // Inefficient, but we save memory for DSP testing!
        TargetFileRead(&x, sizeof(x), 1, fIn);
        y = Sqrt(x);
        TargetFileWrite(&y, sizeof(y), 1, fOut);
    }
}


static void TestSignificantBits(FILE *fIn, FILE *fOut)
{
    DWORD count;
    SWORD x;
    WORD y;

    TargetFileRead(&count, sizeof(count), 1, fIn);

    while(count-- > 0) {
        // Inefficient, but we save memory for DSP testing!
        TargetFileRead(&x, sizeof(x), 1, fIn);
        y = SignificantBits(x);
        TargetFileWrite(&y, sizeof(y), 1, fOut);
    }
}
