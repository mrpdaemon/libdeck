#ifndef _LIBDECK_PRINT_H
#define _LIBDECK_PRINT_H

#include "card.h"
#include "collection.h"
#include "poker.h"

void LibDeck_PrintCard(LibDeckCard *);
void LibDeck_PrintCol(LibDeckCol *);
void LibDeck_PrintPokerClassifyResult(LibDeckPokerClassifyResult *);

#endif /* _LIBDECK_PRINT_H */
