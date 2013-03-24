bin_PROGRAMS = testapp
testapp_SOURCES = testapp.c
testapp_LDADD = libdeck.la -lpthread

lib_LTLIBRARIES = libdeck.la
libdeck_la_CFLAGS = -DHAVE_DEVRANDOM
libdeck_la_SOURCES = card.c collection.c combination.c deck.c libdeck.c poker.c print.c util.c
