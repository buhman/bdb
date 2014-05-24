#pragma once

#include "bdb.h"

int
bdb_tag_find(const char *name, bdb_t *bdb);

int
bdb_tag_add(const char *name, bdb_t *bdb);
