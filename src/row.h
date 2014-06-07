#pragma once

#include "bdb.h"

int
bdb_row_push_str(const uint8_t tag_i, bdb_t *bdb, int nbuf, const char *buf, ...);

int
bdb_row_push(const uint8_t tag_i, const void *buf, const uint64_t size, bdb_t *bdb);

void *
bdb_row_iter(const uint8_t tag_i, uint64_t *next, bdb_t *bdb);
