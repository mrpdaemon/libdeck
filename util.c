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
#else
#include <stdlib.h>
#include <time.h>
#endif /* HAVE_DEVRANDOM */

/* Flag to indicate whether random subsystem is initialized */
int randomInitialized = 0;

#ifdef HAVE_DEVRANDOM
/* Random device location */
#define DEVRANDOM "/dev/urandom"
/* File descriptor for opened random file */
int fdRandom;
#endif /* HAVE_DEVRANDOM */

/*
 * LibDeck_InitRandom --
 *
 *    Initialize the random subsystem.
 *
 * Results:
 *    0 on success, -1 on error.
 *
 * Side effects:
 *    DEVRANDOM could be opened.
 */
int
LibDeck_InitRandom(void)
{
   if (randomInitialized) {
      printf("ERROR: Random subsystem already initialized\n");
      return -1;
   }

#ifdef HAVE_DEVRANDOM
   fdRandom = open(DEVRANDOM, O_RDONLY);

   if (fdRandom < 0) {
      printf("ERROR: Could not open random device %s\n", DEVRANDOM);
      return -1;
   }
#else
   srand(time(NULL));
#endif /* HAVE_DEVRANDOM */

   randomInitialized = 1;

   return 0;
}

/*
 * LibDeck_Random --
 *
 *    Returns a random number modulo 'modulus'.
 *    XXX: Don't need to read full integer length for small modulus
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
#ifdef HAVE_DEVRANDOM
   int randomInt, bytesRead;
#endif /* HAVE_DEVRANDOM */

   if (!randomInitialized) {
      return -1;
   }

#ifdef HAVE_DEVRANDOM
   bytesRead = read(fdRandom, &randomInt, sizeof(int));

   if (bytesRead != sizeof(int)) {
      printf("ERROR: Could not read %d bytes of random data\n", sizeof(int));
      return -1;
   }

   return randomInt % modulus;
#else
   return random() % modulus;
#endif /* HAVE_DEVRANDOM */
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
 *    DEVRANDOM could be closed.
 */
void
LibDeck_CloseRandom(void)
{
#ifdef HAVE_DEVRANDOM
   close(fdRandom);
#endif /* HAVE_DEVRANDOM */
   randomInitialized = 0;
}
