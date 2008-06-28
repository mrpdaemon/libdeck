#include <stdio.h>
#include <stdlib.h>

#include "libdeck.h"

#ifdef SUITE_COMPARE
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
#endif /* SUITE_COMPARE */

int 
main(int argc, char **argv)
{
   LibDeckCol *hand1, *hand2, *hand3, *myDeck, *community, *tmpHand, *hands[3];
   LibDeckPokerResult *result1, *result2, *result3;
   LibDeckCombCtx *combCtx;
   int i, winner, odds[3];

   LibDeck_InitLibrary();
#ifdef SUITE_COMPARE
   LibDeck_RegisterSuiteCompFn(mySuiteCompFn);
#endif /* SUITE_COMPARE */

   myDeck = LibDeck_DeckNew();
   LibDeck_ColShuffle(myDeck, 1000 + LibDeck_UtilRandom(5000));

   printf("Shuffled deck = "); LibDeck_PrintCol(myDeck); printf("\n");
#if 0
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
#endif

   // Pick a 2-hand , 5 community and get the best poker hand out of it
   hands[0] = hand1 = LibDeck_ColPopN(myDeck, 2);
   printf("\nHand 1 = "); LibDeck_PrintCol(hand1);

   hands[1] = hand2 = LibDeck_ColPopN(myDeck, 2);
   printf("\nHand 2 = "); LibDeck_PrintCol(hand2);

   hands[2] = hand3 = LibDeck_ColPopN(myDeck, 2);
   printf("\nHand 3 = "); LibDeck_PrintCol(hand3);
   printf("\n");

   tmpHand = LibDeck_ColNew(5);
   LibDeck_PokerCalcOdds(hands, 3, tmpHand, myDeck, odds);
   printf("\nPre-flop odds of winning:\nHand1: %d%%\nHand2: %d%%\nHand3: %d%%",
          odds[0], odds[1], odds[2]);
   LibDeck_ColFree(tmpHand);

   // FLOP
   community = LibDeck_ColPopN(myDeck, 3);
   printf("\n\nFlop = "); LibDeck_PrintCol(community);
   printf("\n");

   LibDeck_PokerCalcOdds(hands, 3, community, myDeck, odds);
   printf("Post-flop odds of winning:\nHand1: %d%%\nHand2: %d%%\nHand3: %d%%",
          odds[0], odds[1], odds[2]);

   // TURN
   tmpHand = LibDeck_ColPopN(myDeck, 1);
   printf("\n\nTurn = "); LibDeck_PrintCol(tmpHand);
   printf("\n");
   LibDeck_ColAppend(&community, tmpHand);
   LibDeck_ColFree(tmpHand);

   LibDeck_PokerCalcOdds(hands, 3, community, myDeck, odds);
   printf("Post-turn odds of winning:\nHand1: %d%%\nHand2: %d%%\nHand3: %d%%",
          odds[0], odds[1], odds[2]);

   // RIVER
   tmpHand = LibDeck_ColPopN(myDeck, 1);
   printf("\n\nRiver = "); LibDeck_PrintCol(tmpHand);
   LibDeck_ColAppend(&community, tmpHand);
   LibDeck_ColFree(tmpHand);

   printf("\n\nCommunity = "); LibDeck_PrintCol(community);

   result1 = LibDeck_PokerGetBest(hand1, community);
   result2 = LibDeck_PokerGetBest(hand2, community);
   result3 = LibDeck_PokerGetBest(hand3, community);

   printf("\n\nHand 1 made = "); LibDeck_PrintCol(result1->kickerCol);
   printf(" = "); LibDeck_PrintPokerResult(result1);
   printf("\nHand 2 made = "); LibDeck_PrintCol(result2->kickerCol);
   printf(" = "); LibDeck_PrintPokerResult(result2);
   printf("\nHand 3 made = "); LibDeck_PrintCol(result3->kickerCol);
   printf(" = "); LibDeck_PrintPokerResult(result3);

   winner = LibDeck_PokerGetWinner(hands, 3, community);
   switch (winner) {
      case 0:
         printf("\n\nHand 1 wins!\n");
         break;
      case 1:
         printf("\n\nHand 2 wins!\n");
         break;
      case 2:
         printf("\n\nHand 3 wins!\n");
         break;
      default:
         printf("\n\nA draw!\n");
         break;
   }

   LibDeck_PokerFreeResult(result1);
   LibDeck_PokerFreeResult(result2);
   LibDeck_PokerFreeResult(result3);
   LibDeck_ColFree(hand1);
   LibDeck_ColFree(hand2);
   LibDeck_ColFree(hand3);

   LibDeck_ColFree(myDeck);

   LibDeck_DestroyLibrary();

   return 0;
}
