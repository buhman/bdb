#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

#include "bdb.h"
#include "db.h"
#include "header.h"
#include "util.h"

int
bdb_db_open(const char *path, bdb_t *bdb)
{
  int err;
  struct stat s;
  pthread_mutexattr_t attr;

  {
    bdb->fd = open(path, O_RDWR | O_CREAT, 0666);
    if (bdb->fd < 0)
      return bdb->fd;

    err = fstat(bdb->fd, &s);
    if (err < 0)
      return err;
  } /* ... */

  {
    if (s.st_size < (off_t)sizeof(bdb_header_t)) {
      err = ftruncate(bdb->fd, sizeof(bdb_header_t));
      if (err < 0)
        return err;

      bdb->buf = mmap(NULL, sizeof(bdb_header_t), PROT_READ | PROT_WRITE,
                      MAP_SHARED, bdb->fd, 0);
      if (bdb->buf == MAP_FAILED)
        return -1;
      bdb->size = sizeof(bdb_header_t);
      bdb_header_initialize(bdb);
    }
    else {
      bdb->buf = mmap(NULL, s.st_size, PROT_READ | PROT_WRITE,
                      MAP_SHARED, bdb->fd, 0);
      if (bdb->buf == MAP_FAILED)
        return -1;
      bdb->size = s.st_size;
    }
  } /* ... */

  {
    if (!bdb_version_check(bdb)) {
      errno = ENOTSUP;
      bdb_db_close(bdb);
      return -1;
    }
  }

  {
    err = pthread_mutexattr_init(&attr);
    if (err < 0)
      return err;

    err = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
    if (err < 0)
      return err;

    /*
    err = pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
    if (err < 0)
      return err;
    */

    bdb->hlock = malloc(sizeof(pthread_mutex_t));
    err = pthread_mutex_init(bdb->hlock, &attr);
    if (err < 0) {
      free(bdb->hlock);
      return err;
    }

    bdb->rlock = malloc(sizeof(pthread_mutex_t));
    err = pthread_mutex_init(bdb->rlock, &attr);
    if (err < 0) {
      free(bdb->rlock);
      return err;
    }

    err = pthread_mutexattr_destroy(&attr);
    if (err < 0)
      return err;

    bdb->rcount = 0;
  } /* ... */

  return 0;
}

int
bdb_db_close(bdb_t *bdb)
{
  int err;

  err = munmap(bdb->buf, bdb->size);
  if (err < 0)
    return err;

  err = close(bdb->fd);
  if (err < 0)
    return err;

  return 0;
}

int
bdb_db_resize(const size_t size, bdb_t *bdb)
{
  bdb_header_t *h;
  size_t s;
  int err;

  assert(pthread_mutex_lock(bdb->hlock) == 0);
  assert(pthread_mutex_lock(bdb->rlock) == 0);

  h = bdb->buf; /* have hlock */

  if (h->end + size > bdb->size) {

    /* ceiling division */
    s = (1 + ((h->end + size - 1) /
              sysconf(_SC_PAGE_SIZE))) * sysconf(_SC_PAGE_SIZE);

    assert(s > bdb->size);

    err = ftruncate(bdb->fd, s);
    if (err < 0)
      return err;

    bdb->buf = mremap(bdb->buf, bdb->size, s, MREMAP_MAYMOVE);
    if (bdb->buf == MAP_FAILED)
      return -1;

    bdb->size = s;
  }

  assert(pthread_mutex_unlock(bdb->rlock) == 0);
  assert(pthread_mutex_unlock(bdb->hlock) == 0);

  return 0;
}
