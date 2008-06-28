/*
 * libdeck.c - LibDeck library implementation.
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

/*
 * LibDeck_InitLibrary --
 *
 *    Initialize the libdeck library for use.
 *
 * Results:
 *    0 on success, -1 on error.
 *
 * Side effects:
 *    None.
 */
int
LibDeck_InitLibrary(void)
{
   return LibDeck_UtilInitRandom();
}

/*
 * LibDeck_DestroyLibrary --
 *
 *    Destroy an initialized libdeck library.
 *
 * Results:
 *    None.
 *
 * Side effects:
 *    None.
 */
void
LibDeck_DestroyLibrary(void)
{
   LibDeck_UtilCloseRandom();
}
