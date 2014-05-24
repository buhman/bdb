#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "check_bdb.h"

START_TEST (bdb_check_tag_add)
{
  bdb_t *bdb;
  bdb_header_t *h;
  bdb_tag_t *t;

  int err;
  char *name;

  BDB_SETUP("tag.bdb");

  {
    bdb_tag_add("default", bdb);
    fail_unless(err >= 0);

    err = bdb_tag_add("default", bdb);
    fail_unless(err < 0);

    h = (bdb_header_t*)bdb->buf;
    t = (bdb_tag_t*)((char*)bdb->buf + h->tag[0]);
    name = (char*)bdb->buf + t->name;

    fail_unless(strcmp("default", name) == 0);
  } /* ... */

  BDB_CLEAN("tag.bdb");
}
END_TEST

int
main(int argc, char **argv)
{
  int failed;
  Suite *s;
  TCase *tc;
  SRunner *sr;

  s = suite_create("TAG");
  tc = tcase_create("tag");
  tcase_add_test(tc, bdb_check_tag_add);
  suite_add_tcase(s, tc);

  BDB_RUN();
}
