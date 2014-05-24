#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <check.h>
#include <stdio.h>

#include "check_bdb.h"

START_TEST (bdb_check_db)
{
  bdb_t *bdb;
  int err;

  unlink("db.dbd");

  bdb = malloc(sizeof(bdb_t));
  err = bdb_db_open("db.bdb", bdb);
  if (err < 0)
    perror("bdb_db_open");
  fail_unless(err >= 0);

  err = bdb_db_resize(4096, bdb);
  if (err < 0)
    perror("bdb_db_open");
  fail_unless(err >= 0);

  err = bdb_db_close(bdb);
  if (err < 0)
    perror("bdb_db_open");
  fail_unless(err >= 0);
}
END_TEST

int
main(int argc, char **argv)
{
  int failed;
  Suite *s;
  TCase *tc;
  SRunner *sr;

  s = suite_create("DB");
  tc = tcase_create("db");
  tcase_add_test(tc, bdb_check_db);
  suite_add_tcase(s, tc);

  BDB_RUN();
}
