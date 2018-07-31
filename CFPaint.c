#include "CFPaint.h"

void bmpFillCircle(Bmp* b,Color c,int xCenter, int yCenter, int r){
  int x, y;

  /* from xCenter minus radius to xCenter plus radius and yCenter minus radius and yCenter plus radious */
  for(x=xCenter-r; x<xCenter+r; x++) {
    for(y=yCenter-r; y<yCenter+r; y++) {

      /* pinpoint the location of colored dot inside r² */
      if((x - xCenter) * (x - xCenter) + (y - yCenter) * (y - yCenter) <= (r * r)) {
        bmpSetPoint(b, c, x, y);
      }

    }
  }
}


void cfPaintToBmp(CF game,Bmp* b){
  int row, col;
  
  for(row=cfROWS-1; row>=0; row--) {
    for(col=0; col<cfCOLUMNS; col++) {
    	
      if(game.board[col][row] == One) { /* Player One = RED */

        bmpFillCircle(b, RED, 30+col*60, row*60+30, 30);

      } else if(game.board[col][row] == Two) { /* Player Two = WHITE */

        bmpFillCircle(b, WHITE, 30+col*60, row*60+30, 30);

      } else { /* background */

        bmpFillCircle(b, BLACK, 30+col*60, row*60+30, 30);

      }
    }
  }
}