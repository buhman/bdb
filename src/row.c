#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/mman.h>
#include <string.h>

#include "row.h"
#include "util.h"
#include "malloc.h"

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

    buf_o = bdb_malloc(size, bdb);
    if (!buf_o)
      return -1;

  } /* ... */

  {
    dbuf = R(buf_o, bdb);
    memcpy(dbuf, buf, size);
    D(&dbuf, bdb);

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
