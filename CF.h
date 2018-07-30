#ifndef CF__H
#define CF__H
 
#include<stdbool.h>
/*#include "MemoryTest.h"*/
 
/** Constant for the number of columns of the connect four board. **/
#define cfCOLUMNS 7
 
/** Constant for the number of rows of the connect four board.*/
#define cfROWS 6
 
 
/** There are two players. */
typedef enum {None, One, Two} Player;
 
 
/** The next player is the other player or None for the 
    special enum value None. */
Player cfNextPlayer(Player p);
 
 
/** The struct CF describes the status of the game. */
typedef struct {
  /** The board is stored as a two dimension array of player
      valued. The first index signifies the column, the second the row.
      The bottom of a column has row index 0.
  */
  Player** board;
  /** The current player is the player, which does the next move.*/
  Player currentPlayer;
  /** The next attribute stores how many disc have been put
      onto the board.*/
  unsigned int movesDone;
  /** The attribute lastRow stores in which row the last disc landed.
      In the beginning, when no disc is in the board it has value -1.*/
  int lastRow;
    /** The attribute lastRow stores in which column the last disc 
    has been dropped.
      In the beginning, when no disc is in the board it has value -1.*/
  int lastColumn;
} CF;
 
 
/** Creates a new empty game state. First cuurent player is player One.*/
CF cfNewCF();
 
/** Removes a CF value completely from the storage.*/
void cfDeleteCF(CF this);
 
/** Creates a string representation of the board like in the 
  following example:
| | | | | | | |
| | | | | | | |
|O| |X|X| | | |
|X| |O|O| | | |
|X| |O|O| |X| |
|X| |X|O|O|O|X|
 
The character 'O' is player One, the character 'X' player Two.*/
char* cfShowCF(CF this);
 
/** Like cfShowCF, but the memory for the resulting string 
    is provided as parameter.  It needs to have the correct
    size, in order to store the stríng representation of the
    game. */
void cfShowCFIn(CF this,char* result);
 
/** Returns true, if the parameter m is a legal column. Unlike in
    the array index, htecolumns are numbered left two right with
    1 to 7. A column is a legal column to dop a disc, if it is
    between 1 and 7 and not allredy full with discs.*/
bool cfIsMove(CF this,unsigned int m);
 
/** Creates a new game state for this game, with a new disc of
 the current player in the given columm. 
 Remember to set all attributes of the new game state correctly. */
CF cfMakeMove(CF this,unsigned int m) ;
 
 
/* A new generation of follow up game states is an array of
   length 7 representing the 7 columns. Array Elements are 
   pointers of CF. If a column is not a legal move in the game,
   it will be set to NULL in the follow up generation.
*/
typedef CF** Generation;
 
/** For a given game state create the array of follow up
    states. Columns which are allready full of discs will
    result in a null pointer in the generation. */
Generation cfMkNextGeneration(CF this);
 
/** As cfMkNextGeneration but the resulting array of size
    7 is provided as parameter.*/
void cfNextGeneration(CF this,Generation result);
 
/** Delete a generation comletely from the memory. This 
    not only includes the array itself but also all game states 
    CF, the array points to.*/
void cfDeleteGeneration(Generation bs);
 
/** Are 4 discs connected vertical, horizontal or digonal?
    It suffices to check only connections which include the
    last disc thrown.
 */
bool cfHasWinner(CF this);
 
/** Returns true, if all fields of the board have a disc.
    It means that the game ended with a draw.
*/
bool cfFinished(CF this);
 
/**
   This function gives a simple meassure, how good the current state is
   for player p. It counts the three in a row triples of player p, which 
   could be completed to four in a row, and multiplies these with 100.
   Then it counts the two in a row  tuples  of player p, which 
   could be completed to four in a row, and multiplies these with 10.
   Then the same values for the other player are substracted.
    
   The better this function evaluates the state for player p, the better
   the AI will work.
 **/
int cfState(CF this, Player p);
 
 
/** Uses cfState and additionally draws winning states into account.
    Allready implemented in the solution template.*/
int cfEvalState(CF this);
 
/** Gives the best follow up state according to the AI.
    Allready implemented in the solution template.
*/
CF cfBestMove(CF this,int depth);
 
/** The traditional alpha-beta algorithm for game treees. 
    Allready implemented in the solution template.
*/
int cfAlphaBeta(CF this,int tiefe, int alpha, int beta);
 
#endif