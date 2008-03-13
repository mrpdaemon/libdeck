#include <stdio.h>

#include "card.h"
#include "collection.h"
#include "poker.h"

void
LibDeckPrintCardValue(int value)
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
      case LIBDECK_CARD_VALUE_ACE:
         printf("A");
         break;
      default:
         break;
   }
}

void
LibDeckPrintCardSuite(int suite)
{
   switch (suite) {
      case LIBDECK_CARD_SUITE_SPADE:
         printf("S");
         break;
      case LIBDECK_CARD_SUITE_HEART:
         printf("H");
         break;
      case LIBDECK_CARD_SUITE_DIAMOND:
         printf("D");
         break;
      case LIBDECK_CARD_SUITE_CLUB:
         printf("C");
         break;
      default:
         break;
   }
}

void
LibDeck_PrintCard(LibDeckCard *card)
{
   LibDeckPrintCardValue(card->value);
   LibDeckPrintCardSuite(card->suite);
}

void LibDeck_PrintCol(LibDeckCol *collection)
{
   LibDeckCard *curCard;

   printf("[");

   LIBDECK_COL_FORALL(collection, curCard) {
      LibDeck_PrintCard(curCard);
      if (curCard != LibDeck_ColGetLast(collection)) {
         printf(", ");
      }
   }

   printf("]");
}

void LibDeck_PrintPokerClassifyResult(LibDeckPokerClassifyResult *result)
{
   switch (result->handValue) {
      case LIBDECK_POKER_HAND_PAIR:
         printf("Pair of ");
         LibDeckPrintCardValue(result->kicker1.value);
         printf("'s");
         break;
      case LIBDECK_POKER_HAND_TWO_PAIR:
         printf("Two Pair ");
         LibDeckPrintCardValue(result->kicker2.value);
         printf(" and ");
         LibDeckPrintCardValue(result->kicker1.value);
         break;
      case LIBDECK_POKER_HAND_THREE:
         printf("Three of a kind ");
         LibDeckPrintCardValue(result->kicker2.value);
         printf("'s");
         break;
      case LIBDECK_POKER_HAND_STRAIGHT:
         printf("Straight with ");
         LibDeckPrintCardValue(result->kicker1.value);
         printf(" high");
         break;
      case LIBDECK_POKER_HAND_FLUSH:
         printf("Flush");
         break;
      case LIBDECK_POKER_HAND_FULL_HOUSE:
         printf("Full House ");
         LibDeckPrintCardValue(result->kicker2.value);
         printf(" and ");
         LibDeckPrintCardValue(result->kicker1.value);
         break;
      case LIBDECK_POKER_HAND_FOUR:
         printf("Four of a kind ");
         LibDeckPrintCardValue(result->kicker2.value);
         printf("'s");
         break;
      case LIBDECK_POKER_HAND_STR_FLUSH:
         printf("Straight Flush with ");
         LibDeckPrintCardValue(result->kicker1.value);
         printf(" high");
         break;
      default:
         printf("nothing with ");
         LibDeckPrintCardValue(result->kicker1.value);
         printf(" high");
         break;
   }
}
