/*
 * util.h - LibDeck library misc utilities definitions.
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
#ifndef _LIBDECK_UTIL_H
#define _LIBDECK_UTIL_H

int  LibDeck_InitRandom(void);
int  LibDeck_Random(int);
void LibDeck_CloseRandom(void);

#endif /*_LIBDECK_UTIL_H */
