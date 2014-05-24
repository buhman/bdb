#pragma once

#include <assert.h>
#include <stdio.h>

#include "bdb.h"

void *
R(const uint64_t off, bdb_t *bdb);

void
D(void *ptr, bdb_t *bdb);
