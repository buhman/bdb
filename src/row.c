#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/mman.h>
#include <string.h>
#include <stdarg.h>

#include "row.h"
#include "util.h"
#include "malloc.h"

int
bdb_row_push_str(const uint8_t tag_i, bdb_t *bdb, int nbuf, const char *buf, ...)
{
  uint64_t off, *str_a, str_o;
  char *bbuf;
  va_list ap;

  long long count = 0;

  va_start(ap, buf);

  str_o = bdb_malloc(sizeof(uint64_t) * nbuf, bdb);
  if (!str_o)
    return -1;

  while (count < nbuf) {

    if (buf) {
      off = bdb_malloc(strlen(buf) + 1, bdb);
      if (!off)
        return -1;

      bbuf = R(off, bdb);
      strcpy(bbuf, buf);
      D(&bbuf, bdb);
    }
    else
      off = (uint64_t)-1;

    str_a = R(str_o, bdb);
    *(str_a + count) = off;
    D(&str_a, bdb);

    buf = va_arg(ap, const char*);
    count++;
  }

  va_end(ap);

  return bdb_row_push(tag_i, (void*)str_o, (uint64_t)-1, bdb);
}

int
bdb_row_push(const uint8_t tag_i, const void *buf, const uint64_t size, bdb_t *bdb)
{
  bdb_header_t *h;
  uint64_t tag_o, row_o, buf_o;
  bdb_tag_t *tag;
  bdb_row_t *row;
  void *dbuf;

  {
    h = R(0, bdb);
    tag_o = h->tag[tag_i];
    D(&h, bdb);
  } /* ... */

  {
    row_o = bdb_malloc(sizeof(bdb_row_t), bdb);
    if (!row_o)
      return -1;

    if (size != (uint64_t)-1) {
      buf_o = bdb_malloc(size, bdb);
      if (!buf_o)
        return -1;
    }
    else
      buf_o = (uint64_t)buf;
  } /* ... */

  {
    if (size != (uint64_t)-1) {
      dbuf = R(buf_o, bdb);
      memcpy(dbuf, buf, size);
      D(&dbuf, bdb);
    }

    row = R(row_o, bdb);
    row->size = size;
    row->buf = buf_o;

    tag = R(tag_o, bdb);
    row->next = tag->row; /* zero if there is no next row */
    tag->row = row_o;
    D(&tag, bdb);

    D(&row, bdb);
  } /* ... */

  msync(bdb->buf, bdb->size, MS_ASYNC);

  return 0;
}

void *
bdb_row_iter(const uint8_t tag_i, uint64_t *next, bdb_t *bdb)
{
  bdb_header_t *h;
  bdb_row_t *row;
  uint64_t tag_o;
  bdb_tag_t *tag;
  void *buf;

  if (*next == 0)
    return NULL;

  if (*next == (uint64_t)-1) {

    h = R(0, bdb);
    tag_o = h->tag[tag_i];
    D(&h, bdb);

    tag = R(tag_o, bdb);
    *next = tag->row;
    D(&tag, bdb);
  }

  row = R(*next, bdb);
  *next = row->next;
  buf = R(row->buf, bdb);
  D(&row, bdb);

  return buf;
}
