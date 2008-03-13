#include <stdio.h>
#include <stdlib.h>

#include "libdeck.h"

int main(int argc, char **argv)
{
   LibDeckCol *myHand, *myDeck;
   LibDeckPokerClassifyResult *result;
   int i;

   LibDeck_InitLibrary();

   myDeck = LibDeck_DeckNew();

   printf("Deck = "); LibDeck_PrintCol(myDeck);

   LibDeck_ColShuffle(myDeck, 5000);

   printf("\n\nShuffled = "); LibDeck_PrintCol(myDeck);

   for (i = 0; i < 10; i++) {
      myHand = LibDeck_ColPopN(myDeck, 5);
      printf("\n\nHand = "); LibDeck_PrintCol(myHand); printf(" = ");
      result = LibDeck_PokerHandClassify(myHand);
      LibDeck_PrintPokerClassifyResult(result);
      LibDeck_ColFree(myHand);
      free(result);
   }

   printf("\n\nRemaining = "); LibDeck_PrintCol(myDeck);printf("\n");
   LibDeck_ColFree(myDeck);

   return 0;
}
