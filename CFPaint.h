#ifndef CF_PAINT__H
#define CF_PAINT__H
 
#include "CF.h"
#include "BMP.h"
 
void bmpFillCircle(Bmp* b,Color c,int xCenter, int yCenter, int radius);
 
void cfPaintToBmp(CF game,Bmp* b);
 
#endif