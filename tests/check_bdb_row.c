#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "check_bdb.h"

START_TEST (bdb_check_row_push)
{
  bdb_t *bdb;
  int tag, i, err;
  uint64_t next, buf_o;
  char *buf;

  BDB_SETUP("row.bdb");

  {
    tag = bdb_tag_add("foo", bdb);

    err = bdb_row_push(tag, "bar", strlen("bar") + 1, bdb);
    fail_unless(err >= 0);

    err = bdb_row_push(tag, "fooasdf", strlen("fooasdf") + 1, bdb);
    fail_unless(err >= 0);
  } /* ... */

  {
    i = 0;
    next = (uint64_t)-1;

    while ((buf_o = bdb_row_iter(tag, &next, bdb)) != (uint64_t)-1) {

      buf = R(buf_o, bdb);

      if (i == 0)
        fail_unless(strcmp(buf, "fooasdf") == 0);
      if (i == 1)
        fail_unless(strcmp(buf, "bar") == 0);

      D(&buf, bdb);
      i++;
    }

    fail_unless(i == 2);
  } /* ... */

  BDB_CLEAN("row.bdb");
}
END_TEST

int
main(int argc, char **argv)
{
  int failed;
  Suite *s;
  TCase *tc;
  SRunner *sr;

  s = suite_create("ROW");
  tc = tcase_create("row");
  tcase_add_test(tc, bdb_check_row_push);
  suite_add_tcase(s, tc);

  BDB_RUN();
}
