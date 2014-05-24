#pragma once

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>

/* internal/on-disk */

typedef struct bdb_header bdb_header_t;
typedef struct bdb_tag bdb_tag_t;
typedef struct bdb_row bdb_row_t;

struct bdb_header {
  uint8_t version;

  /*void *end*/
  uint64_t end;

  uint8_t tags;

  /*bdb_tag_t *tag[256];*/
  uint64_t tag[256];
};

struct bdb_tag {
  /*uint8_t *name;*/
  uint64_t name;

  /*bdb_row_t *row;*/
  uint64_t row;
};

struct bdb_row {
  /*bdb_row_t *next;*/
  uint64_t next;

  uint64_t size;

  /*void *buf;*/
  uint64_t buf;
};

/* api */

typedef struct bdb bdb_t;

struct bdb {
  int fd;
  void *buf;
  size_t size;
  long long rcount; /* reference count */
  pthread_mutex_t *rlock; /* reference lock */
  pthread_mutex_t *hlock; /* header lock */
};
