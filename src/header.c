#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "version.h"
#include "header.h"
#include "util.h"

void
bdb_header_initialize(bdb_t *bdb)
{
  bdb_header_t *h;

  h = bdb->buf;

  {
    memset(h, 0, sizeof(bdb_header_t));

    h->version = BDB_FORMAT_VERSION;

    h->end = sizeof(bdb_header_t);
  } /* ... */
}

int
bdb_version_check(const bdb_t *bdb)
{
  bdb_header_t *h;

  h = bdb->buf;

  return (h->version == BDB_FORMAT_VERSION);
}
