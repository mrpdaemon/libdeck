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
   int winner, odds[3];

   if (argc < 2) {
	  printf("Usage %s <numThreads>\n", argv[0]);
	  return 1;
   }

   LibDeck_InitLibrary(atoi(argv[1]));
#ifdef SUITE_COMPARE
   LibDeck_RegisterSuiteCompFn(mySuiteCompFn);
#endif /* SUITE_COMPARE */

   myDeck = LibDeck_DeckNew();
   LibDeck_ColShuffle(myDeck, 1000 + LibDeck_UtilRandom(5000));

   printf("Shuffled deck = "); LibDeck_PrintCol(myDeck); printf("\n");

   // Deal 3 hands from the deck
   hands[0] = hand1 = LibDeck_ColPopN(myDeck, 1);
   hands[1] = hand2 = LibDeck_ColPopN(myDeck, 1);
   hands[2] = hand3 = LibDeck_ColPopN(myDeck, 1);
   tmpHand = LibDeck_ColPopN(myDeck, 1);
   LibDeck_ColAppend(&hand1, tmpHand);
   LibDeck_ColFree(tmpHand);
   tmpHand = LibDeck_ColPopN(myDeck, 1);
   LibDeck_ColAppend(&hand2, tmpHand);
   LibDeck_ColFree(tmpHand);
   tmpHand = LibDeck_ColPopN(myDeck, 1);
   LibDeck_ColAppend(&hand3, tmpHand);
   LibDeck_ColFree(tmpHand);

   printf("\nHand 1 = "); LibDeck_PrintCol(hand1);
   printf("\nHand 2 = "); LibDeck_PrintCol(hand2);
   printf("\nHand 3 = "); LibDeck_PrintCol(hand3);
   printf("\n");

   // Calculate pre-flop winning odds for the hands
   tmpHand = LibDeck_ColNew(5);
   LibDeck_PokerCalcOdds(hands, 3, tmpHand, myDeck, odds);
   printf("\nPre-flop odds of winning:\nHand1: %d%%\nHand2: %d%%\nHand3: %d%%",
          odds[0], odds[1], odds[2]);
   LibDeck_ColFree(tmpHand);

   // Burn a card
   LibDeck_ColFree(LibDeck_ColPopN(myDeck, 1));

   // FLOP
   community = LibDeck_ColPopN(myDeck, 3);
   printf("\n\nFlop = "); LibDeck_PrintCol(community);
   printf("\n");

   // Flop odds
   LibDeck_PokerCalcOdds(hands, 3, community, myDeck, odds);
   printf("Post-flop odds of winning:\nHand1: %d%%\nHand2: %d%%\nHand3: %d%%",
          odds[0], odds[1], odds[2]);

   // Burn a card
   LibDeck_ColFree(LibDeck_ColPopN(myDeck, 1));

   // TURN
   tmpHand = LibDeck_ColPopN(myDeck, 1);
   printf("\n\nTurn = "); LibDeck_PrintCol(tmpHand);
   printf("\n");
   LibDeck_ColAppend(&community, tmpHand);
   LibDeck_ColFree(tmpHand);

   // Turn odds
   LibDeck_PokerCalcOdds(hands, 3, community, myDeck, odds);
   printf("Post-turn odds of winning:\nHand1: %d%%\nHand2: %d%%\nHand3: %d%%",
          odds[0], odds[1], odds[2]);

   // Burn a card
   LibDeck_ColFree(LibDeck_ColPopN(myDeck, 1));

   // RIVER
   tmpHand = LibDeck_ColPopN(myDeck, 1);
   printf("\n\nRiver = "); LibDeck_PrintCol(tmpHand);
   LibDeck_ColAppend(&community, tmpHand);
   LibDeck_ColFree(tmpHand);

   printf("\n\nCommunity = "); LibDeck_PrintCol(community);

   // Print what each hand made
   result1 = LibDeck_PokerGetBest(hand1, community);
   result2 = LibDeck_PokerGetBest(hand2, community);
   result3 = LibDeck_PokerGetBest(hand3, community);

   printf("\n\nHand 1 made = "); LibDeck_PrintCol(result1->kickerCol);
   printf(" = "); LibDeck_PrintPokerResult(result1);
   printf("\nHand 2 made = "); LibDeck_PrintCol(result2->kickerCol);
   printf(" = "); LibDeck_PrintPokerResult(result2);
   printf("\nHand 3 made = "); LibDeck_PrintCol(result3->kickerCol);
   printf(" = "); LibDeck_PrintPokerResult(result3);

   // Announce the winner
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
