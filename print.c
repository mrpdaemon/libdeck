#include <stdio.h>

#include "card.h"
#include "collection.h"
#include "poker.h"

void LibDeck_PrintCard(LibDeckCard *card)
{
   switch (card->value) {
      case LIBDECK_CARD_VALUE_TWO ... LIBDECK_CARD_VALUE_TEN:
         printf("%d", card->value);
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

   switch (card->suite) {
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

void LibDeck_PrintPokerHandValue(int classifyResult)
{
   switch (classifyResult) {
      case LIBDECK_POKER_HAND_PAIR:
         printf("Pair");
         break;
      case LIBDECK_POKER_HAND_TWO_PAIR:
         printf("Two Pair");
         break;
      case LIBDECK_POKER_HAND_THREE:
         printf("Three of a kind");
         break;
      case LIBDECK_POKER_HAND_STRAIGHT:
         printf("Straight");
         break;
      case LIBDECK_POKER_HAND_FLUSH:
         printf("Flush");
         break;
      case LIBDECK_POKER_HAND_FULL_HOUSE:
         printf("Full House");
         break;
      case LIBDECK_POKER_HAND_FOUR:
         printf("Four of a kind");
         break;
      case LIBDECK_POKER_HAND_STR_FLUSH:
         printf("Straight Flush");
         break;
      default:
         printf("<nothing>");
         break;
   }
}
