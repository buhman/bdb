#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pthread.h>
#include <signal.h>
#include <stdio.h>

#include "util.h"

void *
R(const uint64_t off, bdb_t *bdb)
{
  assert(pthread_mutex_lock(bdb->hlock) == 0);

  if (bdb->rcount == 0)
    assert(pthread_mutex_lock(bdb->rlock) == 0);

  bdb->rcount++;

  if (off != 0)
    assert(pthread_mutex_unlock(bdb->hlock) == 0);

  return (char*)bdb->buf + off;
}

void
D(void *ptr, bdb_t *bdb)
{
  if ((char*)bdb->buf - *(char**)ptr != 0)
    assert(pthread_mutex_lock(bdb->hlock) == 0);

  bdb->rcount--;

  if (bdb->rcount == 0)
    assert(pthread_mutex_unlock(bdb->rlock) == 0);

  assert(pthread_mutex_unlock(bdb->hlock) == 0);

  *(void**)ptr = NULL;
}
