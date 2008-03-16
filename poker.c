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
 *    'result' is updated with the value / kicker information.
 */
static void
LibDeckPokerUpdateHits(int numHits, // IN: Number of same cards (1=pair 2=trip etc.)
                       LibDeckCard *kicker, // IN: Card to record as kicker
                       LibDeckPokerResult *result) // OUT: Result to update
{
   if (numHits > 0) {
      switch (numHits) {
         case 1:
            // We use kicker1 for the larger pair in case of two-pair
            result->handValue += LIBDECK_POKER_HAND_PAIR;
            if (result->kicker1.value == 0) {
               LibDeck_CardCopy(&result->kicker1, kicker);
            } else {
               LibDeck_CardCopy(&result->kicker2, kicker);
            }
            break;
         case 2:
            // We use the second kicker for three/four of a kind
            result->handValue += LIBDECK_POKER_HAND_THREE;
            LibDeck_CardCopy(&result->kicker2, kicker);
            break;
         case 3:
            result->handValue += LIBDECK_POKER_HAND_FOUR;
            LibDeck_CardCopy(&result->kicker2, kicker);
            break;
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
   int storedValue, numHits;
   LibDeckCard *currentCard, storedCard;
   LibDeckCol *copyCol;

   if (hand->numCards != 5) {
      printf("ERROR: Can not classify, collection size %d != 5\n", hand->numCards);
      return NULL;
   }

   result = (LibDeckPokerResult *) calloc(1, sizeof(LibDeckPokerResult));

   // Take a copy of the input collection since we will be modifying it w/ sort
   copyCol = LibDeck_ColClone(hand);

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
   // Special case A as 1 in [2,3,4,5,A]
   if ((numHits == 4) &&
       (LibDeck_ColGetFirst(copyCol)->value == LIBDECK_CARD_VALUE_TWO) &&
       (LibDeck_ColGetLast(copyCol)->value == LIBDECK_CARD_VALUE_ACE)) {
      numHits = 5;
      // kicker special cased to the 5 as the highest card of the straight
      LibDeck_CardCopy(&result->kicker1, LibDeck_ColGetNth(copyCol, 3));
   }
   if (numHits == 5) {
      result->handValue += LIBDECK_POKER_HAND_STRAIGHT;
      if (result->kicker1.value == 0) {
         // highest card of the straight
         LibDeck_CardCopy(&result->kicker1, LibDeck_ColGetLast(copyCol));
      }
   }

   // Straight, Flush, or Straight+Flush at this point is the final result
   if (result->handValue > 0) {
      goto done;
   }

   // Linear search for same card values
   memset(&storedCard, 0, sizeof(storedCard));
   numHits = 0;
   LIBDECK_COL_FORALL(copyCol, currentCard) {
      if (currentCard->value == storedCard.value) {
         numHits++;
      } else {
         LibDeckPokerUpdateHits(numHits, &storedCard, result);
         LibDeck_CardCopy(&storedCard, currentCard);
         numHits = 0;
      }
   }
   LibDeckPokerUpdateHits(numHits, &storedCard, result);

   // If we got good ol' nothin update kicker with largest card
   if (result->handValue == 0) {
      LibDeck_CardCopy(&result->kicker1, LibDeck_ColGetLast(copyCol));
   }

done:

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
   int comp;

   if (result1->handValue != result2->handValue) {
      if (result1->handValue > result2->handValue) {
         return -1;
      } else {
         return 1;
      }
   }

   // Equal hand values, need to compare kickers selectively
   switch (result1->handValue) {
      case LIBDECK_POKER_HAND_NOTHING:
      case LIBDECK_POKER_HAND_PAIR:
      case LIBDECK_POKER_HAND_STRAIGHT:
      case LIBDECK_POKER_HAND_STR_FLUSH:
         return LibDeck_CardCompare(&result1->kicker1, 
                                    &result2->kicker1);
         break;
      case LIBDECK_POKER_HAND_THREE:
      case LIBDECK_POKER_HAND_FOUR:
         return LibDeck_CardCompare(&result1->kicker2, 
                                    &result2->kicker2);
         break;
      case LIBDECK_POKER_HAND_TWO_PAIR:
      case LIBDECK_POKER_HAND_FULL_HOUSE:
         comp = LibDeck_CardCompare(&result1->kicker2, 
                                    &result2->kicker2);

         if (comp != 0) { // if large kicker is not equal we're done
            return comp;
         }

         return LibDeck_CardCompare(&result1->kicker1, 
                                    &result2->kicker1);
         break;
   }

   // Flush falls through
   return 0;
}
