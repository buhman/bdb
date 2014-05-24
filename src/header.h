#pragma once

#include "bdb.h"

void
bdb_header_initialize(bdb_t *bdb);

int
bdb_version_check(const bdb_t *bdb);
