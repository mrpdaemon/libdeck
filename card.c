#include <string.h>

#include "card.h"
#include "util.h"

void
LibDeck_CardInit(LibDeckCard *card, LibDeckCardSuite suite, LibDeckCardValue value)
{
   card->value = value;	
   card->suite = suite;
}

void
LibDeck_CardInitRandom(LibDeckCard *card)
{
   card->suite = LibDeck_Random(LIBDECK_CARD_NUM_SUITES) + LIBDECK_CARD_SUITE_MIN;
   card->value = LibDeck_Random(LIBDECK_CARD_NUM_VALUES) + LIBDECK_CARD_VALUE_MIN;
}

int
LibDeck_CardCompare(LibDeckCard *c1, LibDeckCard *c2)
{
   if (c1->value > c2->value) {
      return -1;
   }

   if (c1->value < c2->value) {
      return 1;
   }

   return 0;
}

void LibDeck_CardCopy(LibDeckCard *dst, LibDeckCard *src)
{
   memcpy(dst, src, sizeof(LibDeckCard));
}
