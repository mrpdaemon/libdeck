/*
 * print.c - LibDeck printing utilities implementation.
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

#include "card.h"
#include "collection.h"
#include "poker.h"

/*
 * LibDeckPrintCardValue --
 *
 *    Internal method to print the face value of a card.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
static void
LibDeckPrintCardValue(int value) // IN: Card value to print
{
   switch (value) {
      case LIBDECK_CARD_VALUE_TWO ... LIBDECK_CARD_VALUE_TEN:
         printf("%d", value);
         break;
      case LIBDECK_CARD_VALUE_JACK:
         printf("J");
         break;
      case LIBDECK_CARD_VALUE_QUEEN:
         printf("Q");
         break;
      case LIBDECK_CARD_VALUE_KING:
         printf("K");
         break;
      case LIBDECK_CARD_VALUE_ONE:
      case LIBDECK_CARD_VALUE_ACE:
         printf("A");
         break;
      default:
         break;
   }
}

/*
 * LibDeckPrintCardSuite --
 *
 *    Internal method to print the suite of a card.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
static void
LibDeckPrintCardSuite(int suite) // IN: Card suite to print
{
   switch (suite) {
      case LIBDECK_CARD_SUITE_SPADE:
         printf("s");
         break;
      case LIBDECK_CARD_SUITE_HEART:
         printf("h");
         break;
      case LIBDECK_CARD_SUITE_DIAMOND:
         printf("d");
         break;
      case LIBDECK_CARD_SUITE_CLUB:
         printf("c");
         break;
      default:
         break;
   }
}

/*
 * LibDeck_PrintCard --
 *
 *    Prints a two-character ASCII representation of the given card.
 *    For example, king of hearts = KH, five of diamonds = 5D etc.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_PrintCard(LibDeckCard *card) // IN: Card to print
{
   LibDeckPrintCardValue(card->value);
   LibDeckPrintCardSuite(card->suite);
}

/*
 * LibDeck_PrintCol --
 *
 *    Prints two-character ASCII representations of all cards in the
 *    given collection.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void LibDeck_PrintCol(LibDeckCol *collection) // IN: Collection to print
{
   LibDeckCard *curCard;

   printf("{");

   LIBDECK_COL_FORALL(collection, curCard) {
      LibDeck_PrintCard(curCard);
      if (curCard != LibDeck_ColGetLast(collection)) {
         printf(", ");
      }
   }

   printf("}[%d]", collection->numCards);
}

/*
 * LibDeck_PrintPokerResult --
 *
 *    Prints the text representation of the given poker result.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void LibDeck_PrintPokerResult(LibDeckPokerResult *result) // IN: Result to print
{
   switch (result->handValue) {
      case LIBDECK_POKER_HAND_PAIR:
         printf("Pair of ");
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 0)->value);
         printf("'s");
         break;
      case LIBDECK_POKER_HAND_TWO_PAIR:
         printf("Two Pair ");
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 0)->value);
         printf(" and ");
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 2)->value);
         break;
      case LIBDECK_POKER_HAND_THREE:
         printf("Three of a kind ");
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 0)->value);
         printf("'s");
         break;
      case LIBDECK_POKER_HAND_STRAIGHT:
         printf("Straight with ");
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 0)->value);
         printf(" high");
         break;
      case LIBDECK_POKER_HAND_FLUSH:
         printf("Flush ");
         //XXX: Long suite name "Diamonds"
         LibDeckPrintCardSuite(LibDeck_ColGetNth(result->kickerCol, 0)->suite);
         printf(" with ");
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 0)->value);
         printf(" high");
         break;
      case LIBDECK_POKER_HAND_FULL_HOUSE:
         printf("Full House ");
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 0)->value);
         printf(" and ");
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 3)->value);
         break;
      case LIBDECK_POKER_HAND_FOUR:
         printf("Four of a kind ");
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 0)->value);
         printf("'s");
         break;
      case LIBDECK_POKER_HAND_STR_FLUSH:
         printf("Straight Flush ");
         //XXX: Long suite name "Diamonds"
         LibDeckPrintCardSuite(LibDeck_ColGetNth(result->kickerCol, 0)->suite);
         printf(" with ");
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 0)->value);
         printf(" high");
         break;
      default:
         LibDeckPrintCardValue(LibDeck_ColGetNth(result->kickerCol, 0)->value);
         printf(" high");
         break;
   }
}
