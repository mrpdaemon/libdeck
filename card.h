#ifndef _LIBDECK_CARD_H
#define _LIBDECK_CARD_H

typedef enum LibDeckCardSuite {
   LIBDECK_CARD_SUITE_CLUB,
   LIBDECK_CARD_SUITE_DIAMOND,
   LIBDECK_CARD_SUITE_HEART,
   LIBDECK_CARD_SUITE_SPADE
} LibDeckCardSuite;

#define LIBDECK_CARD_SUITE_MIN LIBDECK_CARD_SUITE_CLUB
#define LIBDECK_CARD_SUITE_MAX LIBDECK_CARD_SUITE_SPADE
#define LIBDECK_CARD_NUM_SUITES 4

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

typedef struct LibDeckCard {
   LibDeckCardSuite suite;
   LibDeckCardValue value;
} LibDeckCard;

void LibDeck_CardInit(LibDeckCard *, LibDeckCardSuite, LibDeckCardValue);
void LibDeck_CardInitRandom(LibDeckCard *);
int  LibDeck_CardCompare(LibDeckCard *, LibDeckCard *);
void LibDeck_CardCopy(LibDeckCard *, LibDeckCard *);

#endif /* _LIBDECK_CARD_H */
