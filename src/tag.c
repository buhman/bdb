#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "tag.h"
#include "malloc.h"
#include "util.h"

int
bdb_tag_find(const char *name, bdb_t *bdb)
{
  bdb_header_t *h;
  bdb_tag_t *tag;
  char *n;
  int i;

  h = R(0, bdb);

  for (i = 0; h->tag[i] != 0; i++) {

    tag = (bdb_tag_t*)((char*)bdb->buf + h->tag[i]);
    n = ((char*)bdb->buf + tag->name);
    if (strcmp(name, n) == 0)
      return i;
  }

  D(&h, bdb);

  return -1;
}

int
bdb_tag_add(const char *name, bdb_t *bdb)
{
  uint64_t tag_o, n_o, tags;
  bdb_tag_t *tag;
  bdb_header_t *h;
  size_t len;
  char *n;

  if (bdb_tag_find(name, bdb) >= 0)
    return -1;

  len = strlen(name) + 1;

  {
    tag_o = bdb_malloc(sizeof(bdb_tag_t), bdb);
    if (!tag_o)
      return -1;

    n_o = bdb_malloc(len, bdb);
    if (!n_o)
      return -1;
  } /* ... */

  {
    tag = R(tag_o, bdb);
    n = R(n_o, bdb);
    h = R(0, bdb);

    memcpy(n, name, len);
    tag->name = n - (char*)bdb->buf;
    tag->row = 0;

    h->tag[h->tags] = ((char*)tag - (char*)bdb->buf);
    tags = h->tags++;

    D(&h, bdb);
    D(&n, bdb);
    D(&tag, bdb);
  }

  return tags;
}
