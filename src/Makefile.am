bin_PROGRAMS = testapp
testapp_SOURCES = testapp.c
testapp_LDADD = libdeck.la $(PTHREAD_LIBS)

lib_LTLIBRARIES = libdeck.la
libdeck_la_CFLAGS = $(PTHREAD_CFLAGS)
libdeck_la_SOURCES = card.c collection.c combination.c deck.c libdeck.c poker.c print.c util.c
