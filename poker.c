#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "collection.h"
#include "poker.h"

void
LibDeckPokerUpdateHits(int numHits,
                       LibDeckCard *currentCard,
                       LibDeckPokerClassifyResult *result)
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

LibDeckPokerClassifyResult *
LibDeck_PokerHandClassify(LibDeckCol *hand)
{
   LibDeckPokerClassifyResult *result;
   int storedValue, numHits;
   LibDeckCard *currentCard, storedCard;
   LibDeckCol *copyCol;

   if (hand->numCards != 5) {
      printf("ERROR: Can not classify, collection size %d != 5\n", hand->numCards);
      return NULL;
   }

   result = (LibDeckPokerClassifyResult *) malloc(sizeof(LibDeckPokerClassifyResult));
   memset(result, 0, sizeof(LibDeckPokerClassifyResult));

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
