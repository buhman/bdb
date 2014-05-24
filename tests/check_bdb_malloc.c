#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <check.h>

#include "check_bdb.h"

START_TEST (bdb_check_malloc)
{
  bdb_t *bdb;
  bdb_header_t *h;
  char *ptr;
  uint64_t end;
  size_t size;

  BDB_SETUP("malloc.bdb");

  {
    h = (bdb_header_t*)bdb->buf;
    end = h->end;
    size = bdb->size;

    ptr = R(bdb_malloc(sizeof(bdb_row_t), bdb), bdb);

    fail_unless(h->end >= (ptr - (char*)bdb->buf) + sizeof(bdb_row_t));
    fail_unless((bdb->size >= h->end));
    fail_unless((size >= end) || ((bdb->size > size) && (h->end > end)));

    D(&h, bdb);
  } /* ... */

  BDB_CLEAN("malloc.bdb");
}
END_TEST

int
main(int argc, char **argv)
{
  int failed;
  Suite *s;
  TCase *tc;
  SRunner *sr;

  s = suite_create("MALLOC");
  tc = tcase_create("malloc");
  tcase_add_test(tc, bdb_check_malloc);
  suite_add_tcase(s, tc);

  BDB_RUN();
}
