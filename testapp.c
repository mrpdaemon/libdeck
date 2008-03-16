#include <stdio.h>
#include <stdlib.h>

#include "libdeck.h"

#ifdef SUITE_COMP
#define SuiteComp(suite, value) \
   switch (suite) { \
      case LIBDECK_CARD_SUITE_CLUB: \
         value += 1; \
         break; \
      case LIBDECK_CARD_SUITE_DIAMOND: \
         value += 2; \
         break; \
      case LIBDECK_CARD_SUITE_HEART: \
         value += 3; \
         break; \
      case LIBDECK_CARD_SUITE_SPADE: \
         value += 4; \
         break; \
   }

int
mySuiteCompFn(LibDeckCardSuite s1, LibDeckCardSuite s2)
{
   int value1 = 0, value2 = 0;

   SuiteComp(s1, value1);
   SuiteComp(s2, value2);

   if (value1 > value2) {
      return -1;
   } else if (value1 < value2) {
      return 1;
   }

   return 0;
}
#endif /* SUITE_COMP */

int 
main(int argc, char **argv)
{
   LibDeckCol *hand1, *hand2, *myDeck;
   LibDeckPokerResult *result1, *result2;
   LibDeckCombCtx *combCtx;
   int i, compare;

   LibDeck_InitLibrary();
#ifdef SUITE_COMP
   LibDeck_RegisterSuiteCompFn(mySuiteCompFn);
#endif /* SUITE_COMP */

   myDeck = LibDeck_DeckNew();

   printf("Deck = "); LibDeck_PrintCol(myDeck);

   LibDeck_ColShuffle(myDeck, 1000 + LibDeck_Random(5000));

   printf("\n\nShuffled = "); LibDeck_PrintCol(myDeck); printf("\n");

   // Grab 6 cards and display all possible combinations of 4 from it
   hand1 = LibDeck_ColPopN(myDeck, 6);
   printf("\nCollection = "); LibDeck_PrintCol(hand1);
   hand2 = LibDeck_ColNew(4);
   combCtx = LibDeck_CombNew(hand1, 4, 1);
   i = 0;
   while (LibDeck_CombGetNext(combCtx, hand2)) {
      printf("\nCombination %d = ",++i); LibDeck_PrintCol(hand2);
      LibDeck_ColDiscardN(hand2, 4, 1);
   }
   LibDeck_ColFree(hand1);
   LibDeck_ColFree(hand2);
   printf("\n");

   // 4 iterations of 5 cards each hand, compare 2 poker hands
   for (i = 0; i < 4; i++) {
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
