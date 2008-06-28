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

#ifdef SUITE_COMPARE
/* Registered suite comparison function */
static LibDeckSuiteCompFn suiteCompFn = NULL;
#endif

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
   card->suite = LibDeck_UtilRandom(LIBDECK_CARD_NUM_SUITES) + 
                 LIBDECK_CARD_SUITE_MIN;
   card->value = LibDeck_UtilRandom(LIBDECK_CARD_NUM_VALUES) + 
                 LIBDECK_CARD_VALUE_MIN;
}

#ifdef SUITE_COMPARE
/*
 * LibDeck_RegisterSuiteCompFn --
 *
 *    Register a function to compare two suites for purposes of tiebreaking
 *    on equal card values. By default this function is NULL, meaning that
 *    the library does not take suite into account for determining card
 *    ordering.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_RegisterSuiteCompFn(LibDeckSuiteCompFn compFn) // IN: Function to register
{
   suiteCompFn = compFn;
}

/*
 * LibDeck_SuiteCompare --
 *
 *    Compare two given suites. This is a wrapper that calls the registered
 *    suite comparison function if one exists, or returns 0.
 *
 * Results:
 *    0 if equal, -1 if c1 > c2, 1 if c2 > c1. If not suite comparison function
 *    is registered 0 is returned.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_SuiteCompare(LibDeckCardSuite s1, // IN: First suite to compare
                     LibDeckCardSuite s2) // IN: Second suite to compare
{
   if (suiteCompFn == NULL) {
      return 0;
   }

   return suiteCompFn(s1, s2);
}
#endif /* SUITE_COMPARE */

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

#ifdef SUITE_COMPARE
   if (suiteCompFn != NULL) {
      return suiteCompFn(c1->suite, c2->suite);
   }
#endif

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
