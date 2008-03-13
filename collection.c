#include <stdlib.h>
#include <string.h>

#include "collection.h"
#include "util.h"

LibDeckCol *
LibDeck_ColNew(int maxSize)
{
   if (maxSize <= 0) {
      return NULL;
   }

   LibDeckCol *result = malloc(sizeof(LibDeckCol) + 
                               maxSize * sizeof(LibDeckCard));
   result->numCards = 0;
   result->maxSize = maxSize;

   return result;
}

void
LibDeck_ColFree(LibDeckCol *collection)
{
   free(collection);
}

LibDeckCol *
LibDeck_ColClone(LibDeckCol *collection)
{
   LibDeckCol *result;
   int dataLen = sizeof(LibDeckCol) + 
                 collection->numCards * sizeof(LibDeckCard);

   result = (LibDeckCol *) malloc(sizeof(LibDeckCol) +
                                  collection->maxSize * sizeof(LibDeckCard));
   memcpy(result, collection, dataLen);

   return result;
}

LibDeckCard *
LibDeck_ColGetNth(LibDeckCol *collection, int n)
{
   return &collection->cards[n];
}

LibDeckCard *
LibDeck_ColGetFirst(LibDeckCol *collection)
{
   return LibDeck_ColGetNth(collection, 0);
}

LibDeckCard *
LibDeck_ColGetLast(LibDeckCol *collection)
{
   return LibDeck_ColGetNth(collection, collection->numCards - 1);
}

int
LibDeck_ColAddCard(LibDeckCol *collection, LibDeckCard *card)
{
   if (collection->numCards == collection->maxSize) {
      return -1;
   }

   LibDeck_CardCopy(LibDeck_ColGetNth(collection, collection->numCards), card);

   collection->numCards++;

   return 0;
}

int
LibDeck_ColPop(LibDeckCol *collection, LibDeckCard *card)
{
   if (collection->numCards < 1) {
      return -1;
   }

   memcpy(card, LibDeck_ColGetLast(collection), sizeof(LibDeckCard));

   collection->numCards--;
 
   return 0;
}

LibDeckCol *
LibDeck_ColPopN(LibDeckCol *collection, int n)
{
   LibDeckCol *result;

   if (collection->numCards < n) {
      return NULL;
   }

   result = LibDeck_ColNew(n);

   memcpy(LibDeck_ColGetFirst(result),
          LibDeck_ColGetLast(collection) - (n - 1),
          n * sizeof(LibDeckCard));

   collection->numCards -= n;
   result->numCards += n;

   return result;
}

void
LibDeckColSwapCards(LibDeckCol *collection, int c1, int c2)
{
   LibDeckCard tmpCard;

   if ((c1 > collection->maxSize || c1 < 0) || 
       (c2 > collection->maxSize || c2 < 0) ||
       (c1 == c2)) {
      return;
   }

   LibDeck_CardCopy(&tmpCard, LibDeck_ColGetNth(collection, c1));
   LibDeck_CardCopy(LibDeck_ColGetNth(collection, c1),
                    LibDeck_ColGetNth(collection, c2));
   LibDeck_CardCopy(LibDeck_ColGetNth(collection, c2), &tmpCard);
}

void
LibDeck_ColShuffle(LibDeckCol *collection, int numMoves)
{
   int i;

   for (i = 0; i < numMoves; i++) {
      LibDeckColSwapCards(collection,
                          LibDeck_Random(collection->numCards),
                          LibDeck_Random(collection->numCards));
   }
}

void
LibDeck_ColSort(LibDeckCol *collection)
{
	int i, j;

   for (i = 0; i < (collection->numCards - 1); i++) {
      for (j = i + 1; j < collection->numCards; j++) {
         if (LibDeck_CardCompare(LibDeck_ColGetNth(collection, i),
                                 LibDeck_ColGetNth(collection, j)) < 0) {
            LibDeckColSwapCards(collection, i, j);
         }
      }
   }
}
