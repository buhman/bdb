#include <stdlib.h>
#include <stdio.h>

#include "db.h"
#include "tag.h"
#include "row.h"
#include "util.h"

int
main(int argc, const char **argv)
{
  int err, tag;
  bdb_t *bdb;
  uint64_t next, buf_o;
  char *buf;

  if (argc < 3) {
    fprintf(stderr, "%s: <db.dbd> <tag_name>\n", argv[0]);
    return EXIT_FAILURE;
  }

  bdb = malloc(sizeof(bdb_t));
  err = bdb_db_open(argv[1], bdb);
  if (err < 0) {
    perror("bdb_db_open");
    free(bdb);
    return EXIT_FAILURE;
  }

  tag = bdb_tag_find(argv[2], bdb);
  if (tag < 0) {
    fprintf(stderr, "tag not found");
    free(bdb);
    return EXIT_FAILURE;
  }

  next = (uint64_t)-1;

  while ((buf_o = bdb_row_iter(tag, &next, bdb)) != (uint64_t)-1) {

    buf = R(buf_o, bdb);

    fprintf(stderr, "%s\n", buf);

    D(&buf, bdb);
  }

  free(bdb);
  return EXIT_SUCCESS;
}
