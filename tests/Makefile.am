AM_CFLAGS = -I$(top_srcdir)/src $(LIBCHECK_CFLAGS)
AM_LDFLAGS = $(LIBCHECK_LIBS)

TESTS = \
	check_bdb_db \
	check_bdb_malloc \
	check_bdb_tag \
	check_bdb_row

check_PROGRAMS = \
	check_bdb_db \
	check_bdb_malloc \
	check_bdb_tag \
	check_bdb_row

check_bdb_tag_SOURCES = check_bdb_tag.c
check_bdb_tag_LDADD = $(top_builddir)/src/libbdb.la

check_bdb_malloc_SOURCES = check_bdb_malloc.c
check_bdb_malloc_LDADD = $(top_builddir)/src/libbdb.la

check_bdb_db_SOURCES = check_bdb_db.c
check_bdb_db_LDADD = $(top_builddir)/src/libbdb.la

check_bdb_row_SOURCES = check_bdb_row.c
check_bdb_row_LDADD = $(top_builddir)/src/libbdb.la
