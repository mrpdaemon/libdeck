/*
 * combination.h - LibDeck combinatorics utilities definition.
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
#ifndef _LIBDECK_COMBINATION_H
#define _LIBDECK_COMBINATION_H

#include "collection.h"

/* Combination context structure */
typedef struct LibDeckCombCtx {
   int combSize;            // number of elements per combination
   int done;                // whether all combinations are exhausted
   int copy;                // whether we took a copy of the initial collection
   LibDeckCol *collection;  // pointer to the collection to permute over
   int lastIdxZeroValue;    // last value of idxState[0] for this thread
   int idxState[0];         // array for current indices
} LibDeckCombCtx;

LibDeckCombCtx **LibDeck_CombNew(LibDeckCol *, int, int, int);
int            LibDeck_CombGetNext(LibDeckCombCtx *, LibDeckCol *);
void           LibDeck_CombDestroy(LibDeckCombCtx *);

#endif /* _LIBDECK_COMBINATION_H */
