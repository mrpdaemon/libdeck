/*
 * collection.h - LibDeck card collection definitions.
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
#ifndef _LIBDECK_COLLECTION_H
#define _LIBDECK_COLLECTION_H

#include "card.h"

/* Structure to represent a collection of cards, for example a hand or a deck */
typedef struct LibDeckCol {
   int numCards;           // Current number of cards in the collection
   int maxSize;            // Maximum number of cards that can be placed in
   LibDeckCard cards[0];   // Card array
} LibDeckCol;

/* Macro to iterate through all cards in a collection */
#define LIBDECK_COL_FORALL(colPtr, cardPtr) \
   for (cardPtr = &colPtr->cards[0]; cardPtr != &colPtr->cards[colPtr->numCards]; cardPtr += 1)

LibDeckCol *  LibDeck_ColNew(int);
void          LibDeck_ColFree(LibDeckCol *);
LibDeckCol *  LibDeck_ColClone(LibDeckCol *);

int           LibDeck_ColAddCard(LibDeckCol *, LibDeckCard *);
int           LibDeck_ColPop(LibDeckCol *, LibDeckCard *);
LibDeckCol *  LibDeck_ColPopN(LibDeckCol *, int);

LibDeckCard * LibDeck_ColGetFirst(LibDeckCol *);
LibDeckCard * LibDeck_ColGetLast(LibDeckCol *);
LibDeckCard * LibDeck_ColGetNth(LibDeckCol *, int);

void          LibDeck_ColShuffle(LibDeckCol *, int);
void          LibDeck_ColSort(LibDeckCol *);

#endif /* _LIBDECK_COLLECTION_H */
