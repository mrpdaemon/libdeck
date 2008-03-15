#include <stdio.h>
#include <stdlib.h>

#include "libdeck.h"

int main(int argc, char **argv)
{
   LibDeckCol *hand1, *hand2, *myDeck;
   LibDeckPokerResult *result1, *result2;
   int i, compare;

   LibDeck_InitLibrary();

   myDeck = LibDeck_DeckNew();

   printf("Deck = "); LibDeck_PrintCol(myDeck);

   LibDeck_ColShuffle(myDeck, 1000 + LibDeck_Random(5000));

   printf("\n\nShuffled = "); LibDeck_PrintCol(myDeck); printf("\n");

   for (i = 0; i < 5; i++) {
      hand1 = LibDeck_ColPopN(myDeck, 5);
      hand2 = LibDeck_ColPopN(myDeck, 5);

      printf("\nHand 1 = "); LibDeck_PrintCol(hand1); printf(" = ");
      result1 = LibDeck_PokerClassify(hand1);
      LibDeck_PrintPokerResult(result1);

      printf("\nHand 2 = "); LibDeck_PrintCol(hand2); printf(" = ");
      result2 = LibDeck_PokerClassify(hand2);
      LibDeck_PrintPokerResult(result2);

      compare = LibDeck_PokerCompare(result1, result2);
      if (compare < 0) {
         printf("\nHand 1 wins!\n");
      } else if (compare > 0) {
         printf("\nHand 2 wins!\n");
      } else {
         printf("\nDRAW!\n");
      }

      LibDeck_ColFree(hand1);
      LibDeck_ColFree(hand2);
      free(result1);
      free(result2);
   }

   printf("\nRemaining = "); LibDeck_PrintCol(myDeck);printf("\n");
   LibDeck_ColFree(myDeck);

   LibDeck_DestroyLibrary();

   return 0;
}
