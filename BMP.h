#ifndef BMP__H_
#define BMP__H_

#include <stdlib.h>
#include <stdio.h>

typedef struct {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} Color;

const static Color BLACK = {0,0,0};
const static Color WHITE = {255,255,255};
const static Color RED   = {255,0,0};
const static Color GREEN = {0,255,0};
const static Color BLUE  = {0,0,255};
const static Color YELLOW= {255,255,0};

typedef struct {
  int width;
  int height;
  Color* imagedata ;
} Bmp;

Bmp* newBmp(int width,int  height);
void deleteBmp(Bmp* this);
void bmpWriteToFile(Bmp* this,char* fileName);
void bmpSetPoint(Bmp* b,Color c,int x, int y);

#endif