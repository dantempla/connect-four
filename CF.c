#include "CF.h"

#include <stdlib.h>
#include <stdio.h>

Player cfNextPlayer(Player p){

if(p == None) {
  return One;
} else if(p == One) {
  return Two;
} else /*if(p == Two)*/ {
  return One;
}

}


CF cfNewCF(){
  int i, row, col;
  CF cf;
  Player **board = malloc(cfROWS*sizeof(Player*));
  for(i = 0; i < cfROWS; i++) {
    board[i] = malloc(cfCOLUMNS*sizeof(Player));
  }

  /* fill board with empty Nones */
  for(row = 0; row < cfROWS; row++) {
    for(col = 0; col < cfCOLUMNS; col++) {
      board[row][col] = None;
    }
  }

  cf.board = board;
  cf.currentPlayer = One;
  cf.movesDone = 0;
  cf.lastRow = -1;
  cf.lastColumn = -1;

  return cf;

}  

void cfDeleteCF(CF this){
  int i;
  
  for(i = 0; i < cfROWS; i++) {
    free(this.board[i]);
  }

  free(this.board);
}

void cfDeleteCFptr(CF* this){
  int i;
  
  for(i = 0; i < cfROWS; i++) {
    free(this->board[i]);
  }

  free(this->board);
}

char* cfShowCF(CF this){
  int row;
  int column;
  int cfStringPos = 0;
  char* cfString = malloc(sizeof(char)*96);

  for(row = 0; row < cfROWS; row++) {
    for(column = 0; column < cfCOLUMNS; column++) {

      cfString[cfStringPos++] = '|';

      if(this.board[row][column] == None) {
        cfString[cfStringPos++] = ' ';
      }

      if(this.board[row][column] == One) {
        cfString[cfStringPos++] = 'O';
      }

      if(this.board[row][column] == Two) {
        cfString[cfStringPos++] = 'X';
      }

    }
    
    cfString[cfStringPos++] = '|';
    cfString[cfStringPos++] = '\n';

  }
  cfString[cfStringPos] = '\0';


  return cfString;
}

void cfShowCFIn(CF this,char* result){

  int row;
  int column;
  int cfStringPos = 0;

  for(row = 0; row < cfROWS; row++) {
    for(column = 0; column < cfCOLUMNS; column++) {

      result[cfStringPos++] = '|';

      if(this.board[row][column] == None) {
        result[cfStringPos++] = ' ';
      }

      if(this.board[row][column] == One) {
        result[cfStringPos++] = 'O';
      }

      if(this.board[row][column] == Two) {
        result[cfStringPos++] = 'X';
      }

    }
    
    result[cfStringPos++] = '|';
    result[cfStringPos++] = '\n';

  }
  result[cfStringPos] = '\0';

}


bool cfIsMove(CF this,unsigned int m){

  int row;
  int column;

  for(column = 0; column < cfCOLUMNS; column++) {
    if(m == column) {
      for(row = cfROWS-1; row >= 0; row--) {
        if(this.board[row][column] == None) {
          return true;
        }
      }
    }
  }

  /* Wenn kein Wert  1 bis 7 (0 bis 6) und noch kein Feld Frei ist (None) in der Spalte (Column) */
  return false;

}

CF cfMakeMove(CF this,unsigned int m) {

  int row;
  int col;
  

  CF copy = cfNewCF();
    /* copy the board */
    for(row = 0; row < cfROWS; row++) {
      for(col = 0; col < cfCOLUMNS; col++) {
        copy.board[row][col] = this.board[row][col];
      }
    }
    copy.currentPlayer = this.currentPlayer;
    copy.movesDone = this.movesDone;



  for(col = 0; col < cfCOLUMNS; col++) {
    if(m == col) {
      for(row = cfROWS-1; row >= 0; row--) {
        if(copy.board[row][m] == None) {
          copy.lastRow = row;
          copy.lastColumn = col; /* or m */
          copy.movesDone += 1;
          copy.board[row][col] = copy.currentPlayer;

          copy.currentPlayer = cfNextPlayer(copy.currentPlayer);
          return copy;
        }
      }
    }
  }
  
  return copy;
}

void cfMakeMoveTarget(CF this,unsigned int m, CF* target) {

  int row;
  int col;
  
    /* copy the board */
    for(row = 0; row < cfROWS; row++) {
      for(col = 0; col < cfCOLUMNS; col++) {
        target->board[row][col] = this.board[row][col];
      }
    }
    target->currentPlayer = this.currentPlayer;
    target->movesDone = this.movesDone;



  for(col = 0; col < cfCOLUMNS; col++) {
    if(m == col) {
      for(row = cfROWS-1; row > 0; row--) {
        if(target->board[row][m] == None) {
          target->lastRow = row;
          target->lastColumn = col; /* or m */
          target->movesDone += 1;
          target->board[row][col] = target->currentPlayer;

          target->currentPlayer = cfNextPlayer(target->currentPlayer);
        }
      }
    }
  }
}

Generation cfMkNextGeneration(CF this ){
  Generation gen = malloc(sizeof(CF*)*cfCOLUMNS);
  int i;

  for(i = 0; i < cfCOLUMNS; i++) {
    gen[i] = malloc(sizeof(CF*));
  }

  cfNextGeneration(this, gen);


  return gen;
}

void cfNextGeneration(CF this, Generation g){
  int i;

  for(i = 0; i < cfCOLUMNS; i++) {
    if(cfIsMove(this, i)) {
      //temp = cfMakeMove(this, i);
      //g[i] = &temp;
     *g[i] = cfMakeMove(this,i);

    } else {
      g[i] = NULL;
    }
  }
}

void cfDeleteGeneration(Generation g){

  int i;
  for(i = 0; i < cfCOLUMNS; i++) {

/*    int k;
    for(k = 0; k < cfROWS; k++) {
      cfDeleteCFptr(g[i]);
    }*/

    /*TODO: Clear board rows and g[i]'s*/

    free(&(g[i]->board));

  }

  free(g);
}

bool horizontalCheck(CF this) {
  int row, col;

  for(row = 0; row < cfROWS; row++) {
    for(col = 0; col < cfCOLUMNS-3; col++) {
      if((this.board[row][col] != None) && (this.board[row][col] == this.board[row][col+1]) && (this.board[row][col] == this.board[row][col+2])
        && (this.board[row][col] == this.board[row][col+3])) {
        return true;
      }
    }
  }

  return false;
}

bool verticalCheck(CF this) {
  int row, col;

  for(row = 0; row < cfROWS-3; row++) {
    for(col = 0; col < cfCOLUMNS; col++) {
      if((this.board[row][col] != None) && (this.board[row][col] == this.board[row+1][col]) && (this.board[row][col] == this.board[row+2][col])
        && (this.board[row][col] == this.board[row+3][col])) {
        return true;
      }
    }
  }

  return false;
}

bool diagonalCheck(CF this) {

  int row, col;

  for (col = 0; col <= 3; col++) {
    for (row = 0; row <= 2; row++) {
        if ((this.board[row][col] != None) &&
           (this.board[row][col] == this.board[row+1][col+1]) &&
           (this.board[row][col] == this.board[row+2][col+2]) &&
           (this.board[row][col] == this.board[row+3][col+3]))
               return true;
    }

    for (row = 3; row <= 5; row++) {
        if ((this.board[row][col] != None) &&
           (this.board[row][col] == this.board[row-1][col+1]) &&
           (this.board[row][col] == this.board[row-2][col+2]) &&
           (this.board[row][col] == this.board[row-3][col+3]))
               return true;
    }

  }

  return false;

}

bool cfHasWinner(CF this){
  return (horizontalCheck(this) || verticalCheck(this) || diagonalCheck(this));
}


bool cfFinished(CF this){

  int row;
  int column;

  for(row = 0; row < cfROWS; row++) {
    for(column = 0; column < cfCOLUMNS; column++) {
      if(this.board[row][column] == None)
        return false;
    }
  }

  return true;

}

int cfState(CF this, Player p){

  int row, col;
  int value = 0, streak;

  /* check for 3's/2's horizontal */
  for(row = 0; row < cfROWS; row++) {
    streak = 0;
    for(col = 0; col < cfCOLUMNS; col++) {
      if(this.board[row][col] == p) {
        streak += 1;
      } else {
        if(streak == 2) {
          value += 10;
        } else if(streak == 3) {
          value += 100;
        }
        streak = 0;
      }
    }
  }

  /* check for 3's/2's vertical */
  for(col = 0; col < cfCOLUMNS; col++) {
    streak = 0;
    for(row = 0; row < cfROWS; row++) {
      if(this.board[row][col] == p) {
        streak += 1;
      } else {
        if(streak == 2) {
          value += 10;
        } else if(streak == 3) {
          value += 100;
        }
        streak = 0;
      }
    }
  }

  /* diagonal 3*s */
  for (col = 0; col <= 4; col++) {
    for (row = 0; row <= 3; row++) {
        if ((this.board[row][col] != p) && /* 3 in a row */
           (this.board[row][col] == this.board[row+1][col+1]) &&
           (this.board[row][col] == this.board[row+2][col+2]))
               value += 100;
    }

    for (row = 2; row <= 5; row++) {
        if ((this.board[row][col] != p) &&
           (this.board[row][col] == this.board[row-1][col+1]) &&
           (this.board[row][col] == this.board[row-2][col+2]))
               value += 100;
    }

  }

  /* diagonal 2*s counts 3*s as well as 2x2's -> doe snot matter for algorithm */
  for (col = 0; col <= 5; col++) {
    for (row = 0; row <= 4; row++) {
        if ((this.board[row][col] != p) && /* 3 in a row */
           (this.board[row][col] == this.board[row+1][col+1]))
               value += 10;
    }

    for (row = 1; row <= 5; row++) {
        if ((this.board[row][col] != p) &&
           (this.board[row][col] == this.board[row-1][col+1]))
               value += 10;
    }

  }



return value;

}

int cfEvalState(CF this){
  if (cfHasWinner(this)){
    return -1000000;
  }
  return   cfState(this,cfNextPlayer(this.currentPlayer))
         - cfState(this,this.currentPlayer); 
}



int cfAlphaBeta(CF this,int tiefe, int alpha, int beta){
  if (tiefe==0||cfFinished(this)||cfHasWinner(this))
    return cfEvalState(this);
  
  Generation gs = cfMkNextGeneration(this);
  int m;
  for (m=0;m<cfCOLUMNS;m++){
    if (gs[m]!=NULL){
      int wert = -cfAlphaBeta(*(gs[m]),tiefe-1,-beta,-alpha);
      if (wert >= beta) {alpha= beta;break;}
      if (wert > alpha) alpha = wert;
    } 
  }
  cfDeleteGeneration(gs);
  return alpha;
}


CF cfBestMove(CF this,int depth){
  int val = -1000000;
  CF result;
  Generation gs = cfMkNextGeneration(this);
  int m;
  bool init = false;
  for (m=0;m<cfCOLUMNS;m++){
    if (gs[m]!=NULL){
      CF g = *(gs[m]);
      int eval = -cfAlphaBeta(g,depth,-1000000,-val);
      printf("(%d, %d) ",m+1,eval);
      fflush(stdout);
      if (eval>val){
        result = g;
        val = eval;
        init = true;
      }else if (!init){
        result = g;
        init = true;
      }
    }
  }


  for (m=0;m<cfCOLUMNS;m++){
    if (gs[m]!=NULL){
      if (m!=result.lastColumn){
        cfDeleteCF(*gs[m]);
      }
      free(gs[m]);
    }
  }
  free(gs);
  printf("\n ");

  return result;
}
