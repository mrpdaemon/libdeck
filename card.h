/*
 * card.c - LibDeck card definitions.
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
#ifndef _LIBDECK_CARD_H
#define _LIBDECK_CARD_H

/* Card suite values */
typedef enum LibDeckCardSuite {
   LIBDECK_CARD_SUITE_CLUB,
   LIBDECK_CARD_SUITE_DIAMOND,
   LIBDECK_CARD_SUITE_HEART,
   LIBDECK_CARD_SUITE_SPADE
} LibDeckCardSuite;

#define LIBDECK_CARD_SUITE_MIN LIBDECK_CARD_SUITE_CLUB
#define LIBDECK_CARD_SUITE_MAX LIBDECK_CARD_SUITE_SPADE
#define LIBDECK_CARD_NUM_SUITES 4

/* Card face values */
typedef enum LibDeckCardValue {
   LIBDECK_CARD_VALUE_TWO=2,
   LIBDECK_CARD_VALUE_THREE,
   LIBDECK_CARD_VALUE_FOUR,
   LIBDECK_CARD_VALUE_FIVE,
   LIBDECK_CARD_VALUE_SIX,
   LIBDECK_CARD_VALUE_SEVEN,
   LIBDECK_CARD_VALUE_EIGHT,
   LIBDECK_CARD_VALUE_NINE,
   LIBDECK_CARD_VALUE_TEN,
   LIBDECK_CARD_VALUE_JACK,
   LIBDECK_CARD_VALUE_QUEEN,
   LIBDECK_CARD_VALUE_KING,
   LIBDECK_CARD_VALUE_ACE,
} LibDeckCardValue;

#define LIBDECK_CARD_VALUE_MIN LIBDECK_CARD_VALUE_TWO
#define LIBDECK_CARD_VALUE_MAX LIBDECK_CARD_VALUE_ACE
#define LIBDECK_CARD_NUM_VALUES 13

/* Structure to represent a card */
typedef struct LibDeckCard {
   LibDeckCardSuite suite;
   LibDeckCardValue value;
} LibDeckCard;

/* Prototype for suite comparison function */
typedef int (*LibDeckSuiteCompFn) (LibDeckCardSuite, LibDeckCardSuite);

void LibDeck_CardInit(LibDeckCard *, LibDeckCardSuite, LibDeckCardValue);
void LibDeck_CardInitRandom(LibDeckCard *);

void LibDeck_CardCopy(LibDeckCard *, LibDeckCard *);

int  LibDeck_CardCompare(LibDeckCard *, LibDeckCard *);
int  LibDeck_SuiteCompare(LibDeckCardSuite, LibDeckCardSuite);
void LibDeck_RegisterSuiteCompFn(LibDeckSuiteCompFn);

#endif /* _LIBDECK_CARD_H */
