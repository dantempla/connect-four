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


char* cfShowCF(CF this){
  char* cfString = malloc(sizeof(char)*96);
  cfShowCFIn(this, cfString);


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

  /* If there is still a free frow in a column, return true */
  for(column = 0; column < cfCOLUMNS; column++) {
    if(m == column) {
      for(row = cfROWS-1; row >= 0; row--) {
        if(this.board[row][column] == None) {
          return true;
        }
      }
    }
  }

  /* If no column 0-6 has a free row (None), return false*/
  return false;
}


CF cfMakeMove(CF this,unsigned int m) {

  int row, col;
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


/** Method to calculate potential 2's and 3's horizontally and return a positive int value representing the amount of it */
int horizontalCheckPotential(CF this, Player p) {
  int row, col;
  int value = 0;

  for(row = 0; row < cfROWS; row++) {
    for(col = 0; col < cfCOLUMNS-3; col++) {
      /* _ _ O O */
      if((this.board[row][col+2] == p)
        && (this.board[row][col] == None) && (this.board[row][col+1] == None) && (this.board[row][col+2] == this.board[row][col+3])) {
        value += 10;
      }
      /* _ O O _ */
       if((this.board[row][col+1] == p)
        && (this.board[row][col] == None) && (this.board[row][col+1] == this.board[row][col+2]) 
        && (this.board[row][col+3] == None)) {
        value += 10;
      }
      /* O O _ _ */
       if((this.board[row][col] == p)
        && (this.board[row][col] == this.board[row][col+1]) && (this.board[row][col+2] == None) 
        && (this.board[row][col+3] == None)) {
        value += 10;
      }
      /* _  O O O */
       if((this.board[row][col+1] == p)
        && (this.board[row][col] == None) && (this.board[row][col+1] == this.board[row][col+2])
        && (this.board[row][col+2] == this.board[row][col+3])) {
        value += 100;
      }
      /* O O O _ */
       if((this.board[row][col] == p)
        && (this.board[row][col] == this.board[row][col+1]) && (this.board[row][col+1] == this.board[row][col+2])
        && (this.board[row][col+2] == None)) {
        value += 100;
      }


    }
  }
  return value;
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


/** Method to calculate potential 2's and 3's vertically and return a positive int value representing the amount of it */
int verticalCheckPotential(CF this, Player p) {
  int row, col;
  int value = 0;

  for(row = 0; row < cfROWS-3; row++) {
    for(col = 0; col < cfCOLUMNS; col++) {
      /* _ _ O O */
      if((this.board[row+2][col] == p)
        && (this.board[row][col] == None) && (this.board[row+1][col] == None) && (this.board[row+2][col] == this.board[row+3][col])) {
        value += 10;
      }
      /* _ O O _ */
       if((this.board[row+1][col] == p)
        && (this.board[row][col] == None) && (this.board[row+1][col] == this.board[row+2][col]) 
        && (this.board[row+3][col] == None)) {
        value += 10;
      }
      /* O O _ _ */
       if((this.board[row][col] == p)
        && (this.board[row][col] == this.board[row+1][col]) && (this.board[row+2][col] == None) 
        && (this.board[row+3][col] == None)) {
        value += 10;
      }
      /* _  O O O */
       if((this.board[row+1][col] == p)
        && (this.board[row][col] == None) && (this.board[row+1][col] == this.board[row+2][col])
        && (this.board[row+2][col] == this.board[row+3][col])) {
        value += 100;
      }
      /* O O O _ */
       if((this.board[row][col] == p)
        && (this.board[row][col] == this.board[row+1][col]) && (this.board[row+1][col] == this.board[row+2][col])
        && (this.board[row+3][col] == None)) {
        value += 100;
      }
    }
  }
  return value;
}


bool diagonalCheck(CF this) {

  int row, col;

  /* diagonal bottom right */
  for (col = 0; col <= 3; col++) {
    for (row = 0; row <= 2; row++) {
        if ((this.board[row][col] != None) &&
           (this.board[row][col] == this.board[row+1][col+1]) &&
           (this.board[row][col] == this.board[row+2][col+2]) &&
           (this.board[row][col] == this.board[row+3][col+3]))
               return true;
    }

    /* diagonal top right */
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


/** Method to calculate potential 2's and 3's diagonally and return a positive int value representing the amount of it */
int diagonalCheckPotential(CF this, Player p) {

  int row, col;
  int value = 0;

  /* diagonal bottom right */
  for (col = 0; col <= 3; col++) {
    for (row = 0; row <= 2; row++) {
        /* _ _ O O */
        if ((this.board[row+2][col+2] == p)
        && (this.board[row][col] == None) &&
           (this.board[row+1][col+1] == None) &&
           (this.board[row+2][col+2] == this.board[row+3][col+3]))
               value += 10;

        /* _ O O _ */
         if ((this.board[row+1][col+1] == p)
        && (this.board[row][col] == None) &&
           (this.board[row+1][col+1] == this.board[row+2][col+2]) &&
           (this.board[row+3][col+3] == None))
               value += 10;

        /* O O _ _ */
         if ((this.board[row][col] == p)
        && (this.board[row][col] == this.board[row+1][col+1]) &&
           (this.board[row+2][col+2] == None) &&
           (this.board[row+3][col+3] == None))
               value += 10;

        /* O O O _ */
         if ((this.board[row][col] == p)
        && (this.board[row][col] == this.board[row+1][col+1]) &&
           (this.board[row+1][col+1] == this.board[row+2][col+2]) &&
           (this.board[row+3][col+3] == None))
               value += 100;

        /* _ O O O */
         if ((this.board[row+1][col+1] == p)
        && (this.board[row][col] == None) &&
           (this.board[row+1][col+1] == this.board[row+2][col+2]) &&
           (this.board[row+2][col+2] == this.board[row+3][col+3]))
               value += 100;
    }

    /* diagonal top right */
    for (row = 3; row <= 5; row++) {
        /* _ _ O O */
        if ((this.board[row-2][col+2] == p)
        && (this.board[row][col] == None) &&
           (this.board[row-1][col+1] == None) &&
           (this.board[row-2][col+2] == this.board[row-3][col+3]))
               value += 10;

        /* _ O O _ */
         if ((this.board[row-1][col+1] == p)
        && (this.board[row][col] == None) &&
           (this.board[row-1][col+1] == this.board[row-2][col+2]) &&
           (this.board[row-3][col+3] == None))
               value += 10;

        /* O O _ _ */
         if ((this.board[row][col] == p)
        && (this.board[row][col] == this.board[row-1][col+1]) &&
           (this.board[row-2][col+2] == None) &&
           (this.board[row-3][col+3] == None))
               value += 10;

        /* O O O _ */
         if ((this.board[row][col] == p)
        && (this.board[row][col] == this.board[row-1][col+1]) &&
           (this.board[row-1][col+1] == this.board[row-2][col+2]) &&
           (this.board[row-3][col+3] == None))
               value += 100;

        /* _ O O O */
         if ((this.board[row-1][col+1] == p)
        && (this.board[row][col] == None) &&
           (this.board[row-1][col+1] == this.board[row-2][col+2]) &&
           (this.board[row-2][col+2] == this.board[row-3][col+3]))
               value += 100;
    }
  }
  return value;
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
  return (verticalCheckPotential(this, p) + horizontalCheckPotential(this, p) + diagonalCheckPotential(this, p));
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
