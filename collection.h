#ifndef _LIBDECK_COLLECTION_H
#define _LIBDECK_COLLECTION_H

#include "card.h"

typedef struct LibDeckCol {
   int numCards;
   int maxSize;
   LibDeckCard cards[0];
} LibDeckCol;

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
