/*
 * util.c - LibDeck library misc utilities implementation.
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
#include "util.h"

#include <stdlib.h>
#include <stdio.h>

#ifdef HAVE_DEVRANDOM
#include <fcntl.h>
#include <unistd.h>
#else
#include <time.h>
#endif /* HAVE_DEVRANDOM */

/* Flag to indicate whether random subsystem is initialized */
static int randomInitialized = 0;

#ifdef RESEED_RANDOM
/* Counter to determine whether we should re-seed the generator */
static int reseedCounter;
#endif /* RESEED_RANDOM */

/*
 * LibDeckSeedRandom --
 *
 *    Seed the pseudorandom number generator.
 *
 * Results:
 *    0 on success, -1 on error.
 *
 * Side effects:
 *    None.
 */
static int
LibDeckUtilSeedRandom(void)
{
#ifdef HAVE_DEVRANDOM
   int fdRandom, randomInt, bytesRead;

   fdRandom = open(DEVRANDOM, O_RDONLY);

   if (fdRandom < 0) {
      printf("ERROR: Could not open random device %s\n", DEVRANDOM);
      return -1;
   }

   bytesRead = read(fdRandom, &randomInt, sizeof(int));

   if (bytesRead != sizeof(int)) {
      printf("ERROR: Could not read %d bytes of random data\n", sizeof(int));
      close(fdRandom);
      return -1;
   }

   srand(randomInt);

   close(fdRandom);
#else
   srand(time(NULL));
#endif /* DEVRANDOM */

   return 0;
}
/*
 * LibDeck_UtilInitRandom --
 *
 *    Initialize the random subsystem.
 *
 * Results:
 *    0 on success, -1 on error.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_UtilInitRandom(void)
{
   int result;

   if (randomInitialized) {
      printf("ERROR: Random subsystem already initialized\n");
      return -1;
   }

   result = LibDeckUtilSeedRandom();
   if (result != 0) {
      return result;
   }

#ifdef RESEED_RANDOM
   reseedCounter = 0;
#endif /* RESEED_RANDOM */
   randomInitialized = 1;

   return 0;
}

/*
 * LibDeck_UtilRandom --
 *
 *    Returns a random number modulo 'modulus'.
 *
 * Results:
 *    -1 on error, 0 <= retval < modulus on success.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_UtilRandom(int modulus) // IN: modulus to apply to random result
{
   if (!randomInitialized) {
      return -1;
   }

#ifdef RESEED_RANDOM
   if (reseedCounter == LIBDECK_UTIL_RAND_RESEED) {
      LibDeckUtilSeedRandom();
      reseedCounter = 0;
   } else {
      reseedCounter++;
   }
#endif /* RESEED_RANDOM */

   return random() % modulus;
}

/*
 * LibDeck_UtilCloseRandom --
 *
 *    Close the random subsystem.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_UtilCloseRandom(void)
{
#ifdef RESEED_RANDOM
   reseedCounter = 0;
#endif /* RESEED_RANDOM */
   randomInitialized = 0;
}
