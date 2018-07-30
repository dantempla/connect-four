#include <stdio.h>
#include <stdlib.h>
#include "CF.h"

int main(){
  CF g1 = cfNewCF();
  CF g2;// = cfNewCF();
  char* show = cfShowCF(g1);
  
  while (true){
    cfShowCFIn(g1,show);
    if (cfFinished(g1)){
      printf("Beendet mit Remis\n");
      break;
    }
    printf("\033[H\033[J");
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
    g2 = cfMakeMove(g1,myInt);
    cfDeleteCF(g1);
    g1=g2;
    if (cfHasWinner(g1)){
      printf("Spieler %d hat gewonnen\n",cfNextPlayer(g1.currentPlayer));
      cfDeleteCF(g1);
      break;
    }
  }
  free(show);
  return 0;
}
