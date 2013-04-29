/*
 * poker.h - LibDeck poker library definitions.
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
#ifndef _LIBDECK_POKER_H
#define _LIBDECK_POKER_H

#include "card.h"
#include "collection.h"

// Hand values
#define LIBDECK_POKER_HAND_STR_FLUSH    17 // STRAIGHT + FLUSH
#define LIBDECK_POKER_HAND_FOUR         11
#define LIBDECK_POKER_HAND_FULL_HOUSE   10 // THREE + PAIR
#define LIBDECK_POKER_HAND_FLUSH        9
#define LIBDECK_POKER_HAND_STRAIGHT     8
#define LIBDECK_POKER_HAND_THREE        7
#define LIBDECK_POKER_HAND_TWO_PAIR     6  // 2 * pair
#define LIBDECK_POKER_HAND_PAIR         3
#define LIBDECK_POKER_HAND_NOTHING      0

// Hand classification result structure
typedef struct LibDeckPokerResult {
   int handValue;
   LibDeckCol *kickerCol;
} LibDeckPokerResult;

LibDeckPokerResult   *LibDeck_PokerClassify(LibDeckCol *);

int      LibDeck_PokerCompare(LibDeckPokerResult *,
                              LibDeckPokerResult *);

void     LibDeck_PokerFreeResult(LibDeckPokerResult *);

LibDeckPokerResult *LibDeck_PokerGetBest(LibDeckCol *, LibDeckCol *);

int      LibDeck_PokerGetWinner(LibDeckCol **, int, LibDeckCol *);

int      LibDeck_PokerCalcOdds(LibDeckCol **, int, LibDeckCol *, LibDeckCol *, int *);

#endif /* _LIBDECK_POKER_H */
