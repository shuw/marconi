#include "GlobalDefs.h"

void DSPMult(const SWORD *x, const SWORD *y, SWORD *z, WORD length, WORD shift);
void DSPConv(const SWORD *x, const SWORD *h, WORD impulseLength, SWORD *z, WORD outputCount, WORD shift);
void DSPMagSq(const SWORD *xi, const SWORD *xq, SWORD *z, WORD length, WORD shift);
SWORD DSPMax(const SWORD *x, WORD length, WORD *index);
