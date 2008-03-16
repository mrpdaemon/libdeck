CC=gcc
AR=ar
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

clean:
	rm *.o libdeck.a testapp
