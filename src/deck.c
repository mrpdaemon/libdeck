#include "card.h"
#include "collection.h"

LibDeckCol *
LibDeck_DeckNew(void)
{
   LibDeckCol *newDeck;
   LibDeckCard card;
   int suite, value;

   newDeck = LibDeck_ColNew(LIBDECK_CARD_NUM_SUITES * LIBDECK_CARD_NUM_VALUES);

   for (suite = LIBDECK_CARD_SUITE_MIN; 
        suite < (LIBDECK_CARD_SUITE_MAX + 1); suite++) {
      for (value = LIBDECK_CARD_VALUE_MIN;
           value < (LIBDECK_CARD_VALUE_MAX + 1); value++) {
         LibDeck_CardInit(&card, suite, value);
         LibDeck_ColAddCard(newDeck, &card);
      }
   }

   return newDeck;
}
