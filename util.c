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
#ifdef HAVE_DEVRANDOM
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#else
#include <time.h>
#endif /* HAVE_DEVRANDOM */

/* Flag to indicate whether random subsystem is initialized */
int randomInitialized = 0;

/* Counter to determine whether we should re-seed the generator */
#define RESEED_EVERY_N 10000
int reseedCounter;

#ifdef HAVE_DEVRANDOM
/* Random device location */
#define DEVRANDOM "/dev/urandom"
#endif /* HAVE_DEVRANDOM */

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
int
LibDeckSeedRandom(void)
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
      return -1;
   }

   srand(randomInt);

   close(fdRandom);
#else
   srand(time(NULL));
#endif /* HAVE_DEVRANDOM */

   return 0;
}
/*
 * LibDeck_InitRandom --
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
LibDeck_InitRandom(void)
{
   int result;

   if (randomInitialized) {
      printf("ERROR: Random subsystem already initialized\n");
      return -1;
   }

   result = LibDeckSeedRandom();
   if (result != 0) {
      return result;
   }

   reseedCounter = 0;
   randomInitialized = 1;

   return 0;
}

/*
 * LibDeck_Random --
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
LibDeck_Random(int modulus) // IN: modulus to apply to random result
{
   if (!randomInitialized) {
      return -1;
   }

   if (reseedCounter == RESEED_EVERY_N) {
      LibDeckSeedRandom();
      reseedCounter = 0;
   } else {
      reseedCounter++;
   }

   return random() % modulus;
}

/*
 * LibDeck_CloseRandom --
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
LibDeck_CloseRandom(void)
{
   reseedCounter = 0;
   randomInitialized = 0;
}
