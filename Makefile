CC=gcc
AR=ar
STRIP=strip
CFLAGS=-g -Wall -Werror

LIBOBJS=card.o collection.o combination.o deck.o libdeck.o poker.o print.o util.o
LIBDEFS=-DHAVE_DEVRANDOM
TESTDEFS=

all: libdeck.a testapp

.c.o:
	${CC} ${CFLAGS} ${LIBDEFS} -c $<

libdeck.a: ${LIBOBJS}
	${AR} rcs libdeck.a ${LIBOBJS}

testapp: testapp.c libdeck.a
	${CC} ${CFLAGS} ${TESTDEFS} testapp.c -o testapp libdeck.a
	${STRIP} testapp

clean:
	rm -f *.o libdeck.a testapp
