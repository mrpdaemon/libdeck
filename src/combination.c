/*
 * combination.c - LibDeck combinatorics utilities implementation.
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

#include "collection.h"
#include "combination.h"

/*
 * LibDeck_CombNew --
 *
 *    Allocate, initialize and return a new combination context for purposes
 *    of iterating through all possible combSize length combinations of the
 *    given collection.
 *
 * Results:
 *    Pointer to newly allocated context(es), NULL on error.
 *
 * Side effects:
 *    None.
 */
LibDeckCombCtx **
LibDeck_CombNew(LibDeckCol *collection, // IN: Collection to run combinations on
                int combSize,           // IN: Size of each combination
                int numThreads,         // IN: Number of threads to use
                int copy)               // IN: Take a copy of the collection?
{
   LibDeckCombCtx **result;
   int i, j, idxZeroPerThread, curIdxZero = 0;

   if (combSize > collection->numCards) {
      printf("ERROR: Combination size %d larger than the collection %d\n",
             combSize, collection->numCards);
      return NULL;
   }

   result = calloc(1, sizeof(LibDeckCombCtx *) * numThreads);

   if (result == NULL) {
      printf("ERROR: Failed to allocate combination context.\n");
      return NULL;
   }

   // Compute how many idx[0] values each thread should compute
   if (combSize % numThreads == 0) {
	  idxZeroPerThread = combSize / numThreads;
   } else {
	  idxZeroPerThread = (combSize / numThreads) + 1;
   }

   for (i = 0; i < numThreads; i++) {
      result[i] = calloc(1, sizeof(LibDeckCombCtx) + combSize * sizeof(int));

      if (result[i] == NULL) {
         printf("ERROR: Failed to allocate combination context.\n");
         for (j = 0; j < i; j++) {
            free(result[j]);
         }
         free(result);
         return NULL;
	  }

	  result[i]->combSize = combSize;
	  result[i]->done = 0;

	  curIdxZero += idxZeroPerThread;
	  if (curIdxZero > combSize) {
		 curIdxZero = combSize;
	  }
	  result[i]->lastIdxZeroValue = curIdxZero;

	  if (copy) {
	     result[i]->collection = LibDeck_ColClone(collection);
	     result[i]->copy = 1;
	  } else {
	     result[i]->collection = collection;
	     result[i]->copy = 0;
	  }

	  // Set up initial state to first n cards
	  for (j = 0; j < combSize; j++) {
	     result[i]->idxState[j] = j;
	  }
   }

   return result;
}

/*
 * LibDeck_CombGetNext --
 *
 *    Get the next combination from an active combination context.
 *
 * Results:
 *    1 if a new combination was returned, 0 if we are done.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_CombGetNext(LibDeckCombCtx *combCtx, // IN: Combination context
                    LibDeckCol *colBuf)      // OUT: Buffer to copy combination to
{
   int i, terminate, current, updateVal;

   if ((colBuf->maxSize - colBuf->numCards) < combCtx->combSize) {
      printf("ERROR: Combination buffer does not have enough space.\n");
      return 0;
   }

   if (combCtx->done) {
      return 0;
   }

   // Copy current state as result
   for (i = 0; i < combCtx->combSize; i++) {
      LibDeck_ColAddCard(colBuf, LibDeck_ColGetNth(combCtx->collection,
                                                   combCtx->idxState[i]));
   }

   // Determine where to start trickling updates
   terminate = combCtx->combSize - 1;
   while (terminate >= 0) {
      current = combCtx->idxState[terminate];
      if (current != ((combCtx->collection->numCards - 1) - 
                      (combCtx->combSize - (terminate + 1)))) {
         break;
      }
      terminate--;
   }

   if (terminate < 0) {
      combCtx->done = 1;
   } else {
      // Move forward all indices from terminate on
      updateVal = combCtx->idxState[terminate] + 1;
      for (i = terminate; i < combCtx->combSize; i++) {
         combCtx->idxState[i] = updateVal;
         updateVal++;
      }
   }

   if (terminate == 0 && combCtx->idxState[0] > combCtx->lastIdxZeroValue) {
	  // This thread is done with its work
	  combCtx->done = 1;
   }

   return 1;
}

/*
 * LibDeck_CombDestroy --
 *
 *    Destroy the given combination context
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_CombDestroy(LibDeckCombCtx *combCtx) // IN: Context to destroy
{
   if (combCtx->copy) {
      free(combCtx->collection);
   }

   free(combCtx);
}
