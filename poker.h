#ifndef _LIBDECK_POKER_H
#define _LIBDECK_POKER_H

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
#define LIBDECK_POKER_HAND_NO_PAIR      0

int   LibDeck_PokerHandClassify(LibDeckCol *hand);

#endif /* _LIBDECK_POKER_H */
