/*
 * poker.c - LibDeck poker library implementation.
 *  Copyright (C) 2008  Mark R. Pariente
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collection.h"
#include "combination.h"
#include "poker.h"

/*
 * LibDeckPokerUpdateHits --
 *
 *    Internal method to update a LibDeckPokerResult based on same-card
 *    values.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    The same-value cards are added to kickerCol.
 */
static void
LibDeckPokerUpdateHits(int numHits, // IN: Number of same cards (1=pair 2=trip etc.)
                       LibDeckCard *sameCard, // IN: Last same-value card
                       LibDeckCol *kickerCol, // OUT: Kicker collection to update
                       LibDeckPokerResult *result) // OUT: Result to update
{
   LibDeckCard *tmpCard;

   if (numHits > 0) {
      switch (numHits) {
         case 1:
            result->handValue += LIBDECK_POKER_HAND_PAIR;
            break;
         case 2:
            result->handValue += LIBDECK_POKER_HAND_THREE;
            break;
         case 3:
            result->handValue += LIBDECK_POKER_HAND_FOUR;
            break;
      }

      for (tmpCard = sameCard - numHits; tmpCard != sameCard + 1; tmpCard++) {
         LibDeck_ColAddCard(kickerCol, tmpCard);
      }
   }
}

/*
 * LibDeck_PokerClassify --
 *
 *    Given a poker hand (5 cards) returns a newly allocated result structure
 *    containing the computed hand value and kicker information.
 *
 * Results:
 *    Pointer to the result.
 *
 * Side effects:
 *    Memory is allocated for the result.
 */
LibDeckPokerResult *
LibDeck_PokerClassify(LibDeckCol *hand) // IN: Hand to evaluate
{
   LibDeckPokerResult *result;
   int storedValue, numHits, i, found;
   LibDeckCard *currentCard, *searchCard, storedCard;
   LibDeckCol *copyCol, *kickerCol;

   if (hand->numCards != 5) {
      printf("ERROR: Can not classify, collection size %d != 5\n", hand->numCards);
      return NULL;
   }

   result = (LibDeckPokerResult *) calloc(1, sizeof(LibDeckPokerResult));

   // Take a copy of the input collection since we will be modifying it w/ sort
   copyCol = LibDeck_ColClone(hand);

   // Sort the collection for comparison purposes
   LibDeck_ColSort(copyCol);

   // Flush check
   LIBDECK_COL_FORALL(copyCol, currentCard) {
      if (currentCard == LibDeck_ColGetFirst(copyCol)) {
         storedValue = currentCard->suite;
         numHits = 1;
      } else if (currentCard->suite == storedValue) {
         numHits++;
      } else {
         break;
      }
   }
   if (numHits == 5) {
      result->handValue += LIBDECK_POKER_HAND_FLUSH;
   }

   // Straight check
   LIBDECK_COL_FORALL(copyCol, currentCard) {
      if (currentCard == LibDeck_ColGetFirst(copyCol)) {
         storedValue = currentCard->value;
         numHits = 1;
      } else if (currentCard->value == (storedValue + 1)) {
         storedValue++;
         numHits++;
      } else {
         break;
      }
   }

   // Reverse the collection from this point on
   LibDeck_ColReverse(copyCol);

   // Special case A as 1 : [A,5,4,3,2] -> [5,4,3,2,1]
   if ((numHits == 4) &&
       (LibDeck_ColGetLast(copyCol)->value == LIBDECK_CARD_VALUE_TWO) &&
       (LibDeck_ColGetNth(copyCol, 3)->value == LIBDECK_CARD_VALUE_THREE) &&
       (LibDeck_ColGetFirst(copyCol)->value == LIBDECK_CARD_VALUE_ACE)) {
      numHits = 5;

      for (i = 4; i > 0; i--) {
         LibDeck_ColSwapCards(copyCol, 0, i);
      }
      LibDeck_ColGetLast(copyCol)->value = LIBDECK_CARD_VALUE_ONE;
   }
   if (numHits == 5) {
      result->handValue += LIBDECK_POKER_HAND_STRAIGHT;
   }

   // Straight, Flush, or Straight+Flush at this point is the final result
   if (result->handValue > 0) {
      kickerCol = LibDeck_ColClone(copyCol);
      goto done;
   }

   // Linear search for same card values
   kickerCol = LibDeck_ColNew(5);
   memset(&storedCard, 0, sizeof(storedCard));
   numHits = 0;
   LIBDECK_COL_FORALL(copyCol, currentCard) {
      if (currentCard->value == storedCard.value) {
         numHits++;
      } else {
         LibDeckPokerUpdateHits(numHits, currentCard - 1, kickerCol, result);
         LibDeck_CardCopy(&storedCard, currentCard);
         numHits = 0;
      }
   }
   LibDeckPokerUpdateHits(numHits, currentCard - 1, kickerCol, result);

   // For full house we need to fix kickers like [A,A,3,3,3] -> [3,3,3,A,A]
   if (result->handValue == LIBDECK_POKER_HAND_FULL_HOUSE) {
      if (LibDeck_ColGetNth(kickerCol, 0)->value >
          LibDeck_ColGetNth(kickerCol, 2)->value) {
         LibDeck_ColReverse(kickerCol);
      }
   }

   // Add cards not featured in the kicker to it. (covers nothing case too)
   found = 0;
   LIBDECK_COL_FORALL(copyCol, currentCard) {
      LIBDECK_COL_FORALL(kickerCol, searchCard) {
         if ((searchCard->value == currentCard->value) && 
             (searchCard->suite == currentCard->suite)) {
            found = 1;
            break;
         }
      }

      if (!found) {
         LibDeck_ColAddCard(kickerCol, currentCard);
      }

      found = 0;
   }

done:
   result->kickerCol = kickerCol;

   LibDeck_ColFree(copyCol);
   return result;
}

/*
 * LibDeck_PokerCompare --
 *
 *    Given two hand results compare them to determine which hand wins.
 *
 * Results:
 *    0 on draw, -1 if result1 wins, 1 if result2 wins.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_PokerCompare(LibDeckPokerResult *result1, // IN: First result to compare
                     LibDeckPokerResult *result2) // IN: Second result to compare
{
   int comp, i;

   if (result1->handValue != result2->handValue) {
      if (result1->handValue > result2->handValue) {
         return -1;
      } else {
         return 1;
      }
   }

   // Equal hand values, need to compare kickers
   for (i = 0; i < 5; i++) {
      comp = LibDeck_CardCompare(LibDeck_ColGetNth(result1->kickerCol, i),
                                 LibDeck_ColGetNth(result2->kickerCol, i));
      if (comp != 0) {
         return comp;
      }
   }

   return 0;
}

/*
 * LibDeck_PokerFreeResult --
 *
 *    Free's the given result.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_PokerFreeResult(LibDeckPokerResult *result) // IN: Result to free
{
   free(result->kickerCol);
   free(result);
}

/*
 * LibDeck_PokerTexasGetBest --
 *
 *    Given a 2 card hand and a 5 card community returns the best possible
 *    hand of 5 in form of a result.
 *
 * Results:
 *    Pointer to result, NULL on error.
 *
 * Side effects:
 *    None.
 */
LibDeckPokerResult *
LibDeck_PokerTexasGetBest(LibDeckCol *hand,      // IN: Hand (2 cards)
                          LibDeckCol *community) // IN: Community (5 cards)
{
   LibDeckPokerResult *bestResult = NULL, *currentResult;
   LibDeckCombCtx *combCtx;
   LibDeckCol *combineCol, *handBuf;

   if (hand->numCards != 2) {
      printf("ERROR: Invalid number of hand cards %d != 2\n", hand->numCards);
      return NULL;
   }

   if (community->numCards != 5) {
      printf("ERROR: Invalid number of community cards %d != 5\n", 
             community->numCards);
      return NULL;
   }

   // Get it all in one collection
   combineCol = LibDeck_ColNew(7);
   LibDeck_ColAppend(&combineCol, community);
   LibDeck_ColAppend(&combineCol, hand);

   // Get combinations going on
   handBuf = LibDeck_ColNew(5);
   combCtx = LibDeck_CombNew(combineCol, 5, 1);
   while (LibDeck_CombGetNext(combCtx, handBuf)) {
      currentResult = LibDeck_PokerClassify(handBuf); 
      if (bestResult == NULL) {
         bestResult = currentResult;
      } else {
         if (LibDeck_PokerCompare(currentResult, bestResult) < 0) {
            LibDeck_PokerFreeResult(bestResult);
            bestResult = currentResult;
         } else {
            LibDeck_PokerFreeResult(currentResult);
         }
      }
      LibDeck_ColDiscardN(handBuf, 5, 1);
   }

   LibDeck_ColFree(handBuf);
   LibDeck_ColFree(combineCol);
   LibDeck_CombDestroy(combCtx);

   return bestResult;
}
