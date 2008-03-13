#include <stdio.h>

#include "collection.h"
#include "poker.h"

inline int
LibDeckPokerSameCardValue(int numHits)
{
   switch (numHits) {
      case 1:
         return LIBDECK_POKER_HAND_PAIR;
         break;
      case 2:
         return LIBDECK_POKER_HAND_THREE;
         break;
      case 3:
         return LIBDECK_POKER_HAND_FOUR;
         break;
   }

   return 0;
}

int
LibDeck_PokerHandClassify(LibDeckCol *hand)
{
   int result = 0;
   int storedValue, numHits;
   LibDeckCard *currentCard;
   LibDeckCol *copyCol;

   if (hand->numCards != 5) {
      return -1; // Can only classify collections of size 5
   }

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
      result += LIBDECK_POKER_HAND_FLUSH;
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
   }
   if (numHits == 5) {
      result += LIBDECK_POKER_HAND_STRAIGHT;
   }

   // Straight, Flush, or Straight+Flush at this point is the final result
   if (result > 0) {
      goto done;
   }

   // Linear search for same card values
   storedValue = -1;
   numHits = 0;
   LIBDECK_COL_FORALL(copyCol, currentCard) {
      if (currentCard->value == storedValue) {
         numHits++;
      } else {
         storedValue = currentCard->value;
         if (numHits > 0) {
            result += LibDeckPokerSameCardValue(numHits);
         }
         numHits = 0;
      }
   }
   if (numHits > 0) {
      result += LibDeckPokerSameCardValue(numHits);
   }

done:

   LibDeck_ColFree(copyCol);
   return result;
}
