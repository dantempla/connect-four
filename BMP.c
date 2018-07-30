#include "BMP.h"
#include <stdlib.h>
#include <stdio.h>

Bmp* newBmp(int width,int height){
  Bmp* result = (Bmp*)malloc(sizeof(Bmp));
  result->imagedata =(Color*)malloc(sizeof(Color)*width*height);
  result->width=width;
  result->height=height;
  return result;
}
void deleteBmp(Bmp* this){
  free(this->imagedata);
  free(this);
}

void bmpSetPoint(Bmp* this,Color c,int x, int y){
  if (!(x<0||x>=this->width||y<0||y>=this->height)){
    this->imagedata[y*this->width+x]=c;
  }
}

void putByte(FILE* out,unsigned char ch) {fputc(ch,out);}

void putShort(FILE* out, unsigned short w) {
  putByte(out, w & 0xff);
  putByte(out, (w>>8) & 0xff);
}
void putLong(FILE* out, unsigned int l) {
  putByte(out, l & 0xff);
  putByte(out, (l>>8) & 0xff);
  putByte(out, (l>>16) & 0xff);
  putByte(out, (l>>24) & 0xff);
}
void putColor(FILE* out, Color c) {
  putByte(out,c.blue);
  putByte(out,c.green);
  putByte(out,c.red);
}
int getBytesPerLines(Bmp* this){
  long bytesPerLine = this->width * 3l;  /* (for 24 bit images) */
  bytesPerLine=bytesPerLine+(4-bytesPerLine%4)%4;
  return bytesPerLine;
}
void bmpWriteToFile(Bmp* this,char* fileName) {
  FILE* out = fopen(fileName, "wb");
  if (out == NULL){
    printf("Error opening output file\n");
    exit(1);
  }
  putByte(out,'B'); // "BM"-ID schreiben
  putByte(out,'M');

  int headersize = 54L;
  int bytesPerLine = getBytesPerLines(this);
  int filesize  = headersize+bytesPerLine*this->height;
  putLong(out,filesize);

  putShort(out,0);
  putShort(out,0);
  putLong(out, headersize);  
  putLong(out, 0x28L);       //infoSize
  putLong(out,this-> width);
  putLong(out,this-> height);
  putShort(out, 1);          //biPlanes
  putShort(out, 24);         //bits
  putLong(out,0);            //(no compression)
  putLong(out,0);
  putLong(out,0);
  putLong(out,0);
  putLong(out,0);
  putLong(out,0);
  
  int line, colum;
  for (line=0;line < this->height;line++){
    for (colum=0;colum < this->width;colum++){
      putColor(out,this->imagedata[line*this->width+colum]);
    }
    int missingBytes=bytesPerLine-3*this->width;
    for (;missingBytes>0;missingBytes--)
      putByte(out,0);
  }
  fclose(out);
}