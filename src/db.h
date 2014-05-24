#pragma once

#include "bdb.h"

int
bdb_db_open(const char *path, bdb_t *bdb);

int
bdb_db_close(bdb_t *bdb);

int
bdb_db_resize(const size_t size, bdb_t *bdb);
