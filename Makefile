CC=gcc
AR=ar
CFLAGS=-g -Wall -Werror

LIBOBJS=card.o collection.o deck.o libdeck.o poker.o print.o util.o
LIBDEFS=-DHAVE_DEVRANDOM

all: libdeck.a testapp

.c.o:
	${CC} ${CFLAGS} ${LIBDEFS} -c $<

libdeck.a: ${LIBOBJS}
	${AR} rcs libdeck.a ${LIBOBJS}

testapp: testapp.c libdeck.a
	${CC} ${CFLAGS} testapp.c -o testapp libdeck.a

clean:
	rm *.o libdeck.a testapp
