#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collection.h"
#include "poker.h"

void
LibDeckPokerUpdateHits(int numHits,
                       LibDeckCard *currentCard,
                       LibDeckPokerResult *result)
{
   if (numHits > 0) {
      switch (numHits) {
         case 1:
            result->handValue += LIBDECK_POKER_HAND_PAIR;
            // Update kicker information
            if (result->kicker1.value == 0) {
               LibDeck_CardCopy(&result->kicker1, currentCard);
            } else {
               LibDeck_CardCopy(&result->kicker2, currentCard);
            }
            break;
         case 2:
            result->handValue += LIBDECK_POKER_HAND_THREE;
            LibDeck_CardCopy(&result->kicker2, currentCard);
            break;
         case 3:
            result->handValue += LIBDECK_POKER_HAND_FOUR;
            LibDeck_CardCopy(&result->kicker2, currentCard);
            break;
      }
   }
}

LibDeckPokerResult *
LibDeck_PokerClassify(LibDeckCol *hand)
{
   LibDeckPokerResult *result;
   int storedValue, numHits;
   LibDeckCard *currentCard, storedCard;
   LibDeckCol *copyCol;

   if (hand->numCards != 5) {
      printf("ERROR: Can not classify, collection size %d != 5\n", hand->numCards);
      return NULL;
   }

   result = (LibDeckPokerResult *) malloc(sizeof(LibDeckPokerResult));
   memset(result, 0, sizeof(LibDeckPokerResult));

   // Take a copy of the input collection since we will be modifying it (sort)
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

int
LibDeckPokerIntCompare(int a, int b)
{
   if (a > b) {
      return -1;
   } else if (a < b) {
      return 1;
   }

   return 0;
}

int
LibDeck_PokerCompare(LibDeckPokerResult *result1, LibDeckPokerResult *result2)
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
         return LibDeckPokerIntCompare(result1->kicker1.value, 
                                       result2->kicker1.value);
         break;
      case LIBDECK_POKER_HAND_THREE:
      case LIBDECK_POKER_HAND_FOUR:
         return LibDeckPokerIntCompare(result1->kicker2.value, 
                                       result2->kicker2.value);
         break;
      case LIBDECK_POKER_HAND_TWO_PAIR:
      case LIBDECK_POKER_HAND_FULL_HOUSE:
         comp = LibDeckPokerIntCompare(result1->kicker2.value, 
                                        result2->kicker2.value);

         if (comp != 0) { // if large kicker is not equal we're done
            return comp;
         }

         return LibDeckPokerIntCompare(result1->kicker1.value, 
                                       result2->kicker1.value);
         break;
   }

   // Flush falls through
   return 0;
}
