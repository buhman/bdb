#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>

#include "malloc.h"
#include "db.h"
#include "util.h"

uint64_t
bdb_malloc(const size_t size, bdb_t *bdb)
{
  bdb_header_t *h;
  int err;
  uint64_t off;

  err = bdb_db_resize(size, bdb);
  if (err < 0)
    return 0;

  assert(!pthread_mutex_lock(bdb->hlock));

  h = bdb->buf; /* have hlock */

  off = h->end;
  h->end += size;

  assert(!pthread_mutex_unlock(bdb->hlock));

  return off;
}
