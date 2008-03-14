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
   LibDeckCard kicker1;
   LibDeckCard kicker2;
} LibDeckPokerResult;

LibDeckPokerResult   *LibDeck_PokerClassify(LibDeckCol *);

int      LibDeck_PokerCompare(LibDeckPokerResult *, 
                              LibDeckPokerResult *);

#endif /* _LIBDECK_POKER_H */
