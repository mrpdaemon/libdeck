/*
 * card.c - LibDeck card implementation.
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
#include <string.h>

#include "card.h"
#include "util.h"

/*
 * LibDeck_CardInit --
 *
 *    Initialize the given card buffer with the given value/suite
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_CardInit(LibDeckCard *card,      // OUT: Card buffer to initialize
                 LibDeckCardSuite suite, // IN: Suite to initialize to
                 LibDeckCardValue value) // IN: Value to initialize to
{
   card->value = value;	
   card->suite = suite;
}

/*
 * LibDeck_CardInitRandom --
 *
 *    Initialize the given card buffer to a random value/suite
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_CardInitRandom(LibDeckCard *card) // OUT: Card buffer to initialize
{
   card->suite = LibDeck_Random(LIBDECK_CARD_NUM_SUITES) + LIBDECK_CARD_SUITE_MIN;
   card->value = LibDeck_Random(LIBDECK_CARD_NUM_VALUES) + LIBDECK_CARD_VALUE_MIN;
}

/*
 * LibDeck_CardCompare --
 *
 *    Compare two given cards.
 *
 * Results:
 *    0 if equal, -1 if c1 > c2, 1 if c2 > c1.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_CardCompare(LibDeckCard *c1, // IN: First card to compare
                    LibDeckCard *c2) // IN: Second card to compare
{
   if (c1->value > c2->value) {
      return -1;
   }

   if (c1->value < c2->value) {
      return 1;
   }

   return 0;
}

/*
 * LibDeck_CardCopy --
 *
 *    Copy from a given card buffer into a given card buffer.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void LibDeck_CardCopy(LibDeckCard *dst, // OUT: Card buffer to copy to
                      LibDeckCard *src) // IN: Card buffer to copy from
{
   memcpy(dst, src, sizeof(LibDeckCard));
}
