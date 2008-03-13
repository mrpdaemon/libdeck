CC=gcc
CFLAGS=-g -Wall -Werror

LIBOBJ=card.o collection.o deck.o libdeck.o poker.o print.o util.o

all: ${LIBOBJ} testapp

clean:
	rm ${LIBOBJ} testapp

card.o: card.c card.h
	${CC} ${CFLAGS} -c card.c

collection.o: collection.c collection.h
	${CC} ${CFLAGS} -c collection.c

deck.o: deck.c deck.h
	${CC} ${CFLAGS} -c deck.c

libdeck.o: libdeck.c libdeck.h
	${CC} ${CFLAGS} -c libdeck.c

print.o: print.c print.h
	${CC} ${CFLAGS} -c print.c

poker.o: poker.c poker.h
	${CC} ${CFLAGS} -c poker.c

util.o: util.c util.h
	${CC} ${CFLAGS} -c util.c

testapp: testapp.c ${LIBOBJ}
	${CC} ${CFLAGS} testapp.c -o testapp ${LIBOBJ} 
