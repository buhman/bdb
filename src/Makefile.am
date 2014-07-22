AM_CFLAGS = $(PTHREAD_CFLAGS)
LIBS = $(PTHREAD_LIBS)

EXTRA_DIST = \
	version.h.in

lib_LTLIBRARIES = libbdb.la

libbdb_la_SOURCES = \
	version.h \
	bdb.h \
	util.c util.h \
	header.c header.h \
	malloc.c malloc.h \
	db.c db.h \
	tag.h tag.c \
	row.h row.c

libbdb_la_LDFLAGS = $(AM_LDFLAGS) -version-info $(LIBBDB_VERSION_INFO)
