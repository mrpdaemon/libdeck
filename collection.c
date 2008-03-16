/*
 * collection.c - LibDeck card collection implementation.
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
#include <stdlib.h>
#include <string.h>

#include "collection.h"
#include "util.h"

/*
 * LibDeck_ColNew --
 *
 *    Allocate and return a new empty card collection with maxSize capacity.
 *
 * Results:
 *    Pointer to the new collection, NULL on error.
 *
 * Side effects:
 *    None.
 */
LibDeckCol *
LibDeck_ColNew(int maxSize) // IN: Maximum size of the collection
{
   if (maxSize <= 0) {
      return NULL;
   }

   LibDeckCol *result = calloc(1, sizeof(LibDeckCol) + 
                               maxSize * sizeof(LibDeckCard));
   if (!result) {
      printf("ERROR: Failed to allocate collection, out of memory\n");
      return NULL;
   }

   result->numCards = 0;
   result->maxSize = maxSize;

   return result;
}

/*
 * LibDeck_ColFree --
 *
 *    Free the given collection.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_ColFree(LibDeckCol *collection) // IN/OUT: Collection to free
{
   free(collection);
}

/*
 * LibDeck_ColClone --
 *
 *    Clone the given collection, creating a newly allocated copy.
 *
 * Results:
 *    Pointer to the new collection.
 *
 * Side effects:
 *    None.
 */
LibDeckCol *
LibDeck_ColClone(LibDeckCol *collection) // IN: Collection to clone
{
   LibDeckCol *result;
   int dataLen = sizeof(LibDeckCol) + 
                 collection->numCards * sizeof(LibDeckCard);

   result = (LibDeckCol *) malloc(sizeof(LibDeckCol) +
                                  collection->maxSize * sizeof(LibDeckCard));
   if (!result) {
      printf("ERROR: Failed to clone collection, out of memory\n");
      return NULL;
   }

   memcpy(result, collection, dataLen);

   return result;
}

/*
 * LibDeck_ColGetNth --
 *
 *    Returns a pointer to the Nth element of the given collection.
 *
 * Results:
 *    Pointer to the Nth card.
 *
 * Side effects:
 *    None.
 */
LibDeckCard *
LibDeck_ColGetNth(LibDeckCol *collection, // IN: Collection to search
                  int n)                  // IN: Index to look for
{
   return &collection->cards[n];
}

/*
 * LibDeck_ColGetFirst --
 *
 *    Returns a pointer to the 1st element of the given collection.
 *
 * Results:
 *    Pointer to the 1st card.
 *
 * Side effects:
 *    None.
 */
LibDeckCard *
LibDeck_ColGetFirst(LibDeckCol *collection) // IN: Collectio to seach
{
   return LibDeck_ColGetNth(collection, 0);
}

/*
 * LibDeck_ColGetLast --
 *
 *    Returns a pointer to the last element of the given collection.
 *
 * Results:
 *    Pointer to the last card.
 *
 * Side effects:
 *    None.
 */
LibDeckCard *
LibDeck_ColGetLast(LibDeckCol *collection) // IN: Collection to search
{
   return LibDeck_ColGetNth(collection, collection->numCards - 1);
}

/*
 * LibDeck_ColAddCard --
 *
 *    Adds the given card to the given collection
 *
 * Results:
 *    0 on success, -1 on error.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_ColAddCard(LibDeckCol *collection, // IN: Collection to add to
                   LibDeckCard *card)      // IN: Card to add
{
   if (collection->numCards == collection->maxSize) {
      return -1;
   }

   LibDeck_CardCopy(LibDeck_ColGetNth(collection, collection->numCards), card);

   collection->numCards++;

   return 0;
}

/*
 * LibDeck_ColPop --
 *
 *    Remove the top (last) card from the given collection and copy the card
 *    to the buffer given in 'card'.
 *
 * Results:
 *    0 on success, -1 on error.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_ColPop(LibDeckCol *collection, // IN: Collection to pop from
               LibDeckCard *card)      // OUT: Pointer for copying popped card
{
   if (collection->numCards < 1) {
      return -1;
   }

   memcpy(card, LibDeck_ColGetLast(collection), sizeof(LibDeckCard));

   collection->numCards--;
 
   return 0;
}

/*
 * LibDeck_ColPopN --
 *
 *    Remove the top N cards from the given collection, put them in a newly
 *    allocated collection and return it.
 *
 * Results:
 *    Pointer to the newly allocated collection, NULL on error.
 *
 * Side effects:
 *    None.
 */
LibDeckCol *
LibDeck_ColPopN(LibDeckCol *collection, // IN: Collection to pop from
                int n)                  // IN: Number of cards to pop
{
   LibDeckCol *result;

   if (collection->numCards < n) {
      printf("ERROR: Not enough cards (%d) to pop %d from\n",
             collection->numCards, n);
      return NULL;
   }

   result = LibDeck_ColNew(n);
   if (!result) {
      return NULL;
   }

   memcpy(LibDeck_ColGetFirst(result),
          LibDeck_ColGetLast(collection) - (n - 1),
          n * sizeof(LibDeckCard));

   collection->numCards -= n;
   result->numCards += n;

   return result;
}

/*
 * LibDeck_ColSwapCards --
 *
 *    Internal method to swap two cards from a collection.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_ColSwapCards(LibDeckCol *collection, // IN: Collection to swap from
                     int c1,                 // IN: Index of first card to swap
                     int c2)                 // IN: Index of second card to swap
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

/*
 * LibDeck_ColShuffle --
 *
 *    Shuffle the given collection using numMoves random swaps.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_ColShuffle(LibDeckCol *collection,  // IN: Collection to shuffle
                   int numMoves)            // IN: Number of random swaps
{
   int i;

   for (i = 0; i < numMoves; i++) {
      LibDeck_ColSwapCards(collection,
                          LibDeck_Random(collection->numCards),
                          LibDeck_Random(collection->numCards));
   }
}

/*
 * LibDeck_ColSort --
 *
 *    Sort the given collection based on card values using insertion sort.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_ColSort(LibDeckCol *collection) // IN: Collection to sort
{
	int i, j;

   for (i = 0; i < (collection->numCards - 1); i++) {
      for (j = i + 1; j < collection->numCards; j++) {
         if (LibDeck_CardCompare(LibDeck_ColGetNth(collection, i),
                                 LibDeck_ColGetNth(collection, j)) < 0) {
            LibDeck_ColSwapCards(collection, i, j);
         }
      }
   }
}

/*
 * LibDeck_ColReverse --
 *
 *    Reverses the given collection.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_ColReverse(LibDeckCol *collection) // IN: Collection to reverse
{
	int i;

   for (i = 0; i < (collection->numCards / 2); i++) {
      LibDeck_ColSwapCards(collection, i, collection->numCards - (i + 1));
   }
}

/*
 * LibDeck_ColDiscardN --
 *
 *    Discard N cards from top of the given collection.
 *
 * Results:
 *    0 on success, -1 on error.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_ColDiscardN(LibDeckCol *collection, // IN: Collection to sort
                    int n,                  // IN: Number of cards to discard
                    int zero)               // IN: Whether to zero out card data
{
   if (n > collection->numCards) {
      printf("ERROR: Can not discard %d cards, collection only has %d\n",
             n, collection->numCards);
      return -1;
   }

   collection->numCards -= n;

   if (zero) {
      memset(LibDeck_ColGetNth(collection, collection->numCards),
             0, n * sizeof(LibDeckCard));
   }

   return 0;
}

/*
 * LibDeck_ColAppend --
 *
 *    Append the contents of a collection into another collection.
 *    //XXX: memory notes
 *
 * Results:
 *    0 on success, -1 on error.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_ColAppend(LibDeckCol **appendTo,  // IN: Collection to append to
                  LibDeckCol *appendFrom) // IN: Collection to append from
{
   LibDeckCol *tmpAppendTo = *appendTo;

   if ((tmpAppendTo->maxSize - tmpAppendTo->numCards) < appendFrom->numCards) {
      // Need to reallocate
      tmpAppendTo = realloc(tmpAppendTo, 
                            sizeof(LibDeckCol) +
                            (tmpAppendTo->numCards + appendFrom->numCards) * 
                            sizeof(LibDeckCard));

      if (tmpAppendTo == NULL) {
         printf("ERROR: Failed to realloc new collection\n");
         return -1;
      }

      *appendTo = tmpAppendTo;
   }

   memcpy(LibDeck_ColGetNth(tmpAppendTo, tmpAppendTo->numCards),
          LibDeck_ColGetFirst(appendFrom),
          appendFrom->numCards * sizeof(LibDeckCard));

   tmpAppendTo->numCards += appendFrom->numCards;

   return 0;
}
