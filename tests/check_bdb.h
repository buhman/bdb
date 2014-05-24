#pragma once

#include <check.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

#include "bdb.h"
#include "db.h"
#include "malloc.h"
#include "tag.h"
#include "row.h"
#include "util.h"

#define BDB_SETUP(_n)                           \
  do {                                          \
    bdb = malloc(sizeof(bdb_t));                \
    unlink(_n);                         \
    bdb_db_open(_n, bdb);                  \
 } while (0);

#define BDB_CLEAN(_n)                             \
  do {                                          \
    bdb = malloc(sizeof(bdb_t));                \
    unlink(_n);                         \
    bdb_db_open(_n, bdb);                  \
  } while (0);

#define BDB_RUN()                               \
  do {                                          \
    sr = srunner_create(s);                     \
    srunner_run_all(sr, CK_NORMAL);             \
    failed = srunner_ntests_failed(sr);         \
    srunner_free(sr);                           \
    if (failed)                                 \
      return EXIT_FAILURE;                      \
    else                                        \
      return EXIT_SUCCESS;                      \
  } while (0);
