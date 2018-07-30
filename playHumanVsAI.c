#include <stdio.h>
#include <stdlib.h>
#include "CF.h"

int main(){
  CF g1 = cfNewCF();
  CF g2;
  char* show = cfShowCF(g1);
  
  while (true){
    printf("\033[H\033[J");
    cfShowCFIn(g1,show);
    if (cfFinished(g1)){
      printf("%s\n\nBeendet mit Remis\n",show);
      cfDeleteCF(g1);
      break;
    }
    printf("%s\nEs ist Spieler %d am Zug.",show,g1.currentPlayer);
    int myInt;
    bool isMove=false;

    do{
      printf("In welche Spalte soll gesetzt werde? ");
      scanf("%d", &myInt);
      printf("Ihr Zug: %d\n",myInt);

      isMove = cfIsMove(g1,myInt);
      if (!isMove){
	printf("%d ist keine gültige Spalte für einen Zug\n",myInt);
      }
    }while(!isMove);
    printf("\033[H\033[J");

    
    g2 = cfMakeMove(g1,myInt);
    cfDeleteCF(g1);
    g1=g2;
    if (cfHasWinner(g1)){
      printf("\033[H\033[J");
      cfShowCFIn(g1,show);
      printf("%s\nSpieler %d hat gewonnen\n",show,cfNextPlayer(g1.currentPlayer));
      cfDeleteCF(g1);
      break;
    }
    printf("\033[H\033[J");
    cfShowCFIn(g1,show);
    printf("%s\nDer Computer ist am Zug.\n",show);

    g2 = cfBestMove(g1,9);
    cfDeleteCF(g1);
    g1=g2;
    if (cfHasWinner(g1)){
      printf("\033[H\033[J");
      cfShowCFIn(g1,show);
      printf("%s\nSpieler %d hat gewonnen\n",show,cfNextPlayer(g1.currentPlayer));
      cfDeleteCF(g1);
      break;
    }
  }
  free(show);
  /*printStorage(); */
  return 0;
}
