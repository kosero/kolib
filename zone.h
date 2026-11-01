#ifndef ZONE_H
#define ZONE_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

enum { ZONE_STATIC = 1, ZONE_LEVEL = 50, ZONE_PURGABLE = 100 };

typedef struct Zone Zone;

Zone *zone_create(size_t chunk_size, size_t max_total);
void zone_destroy(Zone *z);

void *zone_alloc(Zone *z, size_t size, int tag, void **user);
void *zone_zalloc(Zone *z, size_t size, int tag, void **user);
void *zone_alloc_array(Zone *z, size_t count, size_t elem, int tag,
                       void **user);
void *zone_realloc(Zone *z, void *p, size_t size);

void zone_free(Zone *z, void *p);
void zone_free_tags(Zone *z, int lo, int hi);

int zone_change_tag(void *p, int tag);
int zone_set_owner(void *p, void **user);

char *zone_strdup(Zone *z, const char *s, int tag, void **user);
char *zone_printf(Zone *z, int tag, void **user, const char *fmt, ...);

size_t zone_used(const Zone *z);
size_t zone_reserved(const Zone *z);
const char *zone_check(const Zone *z);

#define ZONE_NEW(z, T, tag) ((T *)zone_zalloc((z), sizeof(T), (tag), NULL))
#define ZONE_NEW_ARRAY(z, T, n, tag)                                           \
  ((T *)zone_alloc_array((z), (n), sizeof(T), (tag), NULL))

#ifdef __cplusplus
}
#endif
#endif // ZONE_H

#ifdef ZONE_IMPLEMENTATION
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ZALIGN 16u
#define ZMAGIC 0x5A4F4E45u
#define ZT_FREE 0
#define ZT_SENTINEL (-1)

typedef struct ZBlock {
  size_t size;
  struct ZBlock *prev;
  void **user;
  int tag;
  uint32_t magic;
} ZBlock;

typedef struct ZChunk {
  struct ZChunk *next;
  size_t bytes;
} ZChunk;

struct Zone {
  ZChunk *chunks;
  ZBlock *rover;
  size_t chunk_size, max_total, total, used;
};

#define Z_ROUND(n) (((size_t)(n) + (ZALIGN - 1)) & ~(size_t)(ZALIGN - 1))
#define Z_HDR Z_ROUND(sizeof(ZBlock))
#define Z_CHDR Z_ROUND(sizeof(ZChunk))
#define Z_MINBLOCK (Z_HDR + ZALIGN)

static ZBlock *z_next(ZBlock *b) { return (ZBlock *)((char *)b + b->size); }
static ZBlock *z_first(ZChunk *c) { return (ZBlock *)((char *)c + Z_CHDR); }
static ZChunk *z_chunk_of_first(ZBlock *b) {
  return (ZChunk *)((char *)b - Z_CHDR);
}
static ZChunk *z_sentinel_chunk(ZBlock *s) { return (ZChunk *)(void *)s->user; }

static ZBlock *z_header(void *p) {
  ZBlock *b;
  if (!p)
    return NULL;
  b = (ZBlock *)((char *)p - Z_HDR);
  if (b->magic != ZMAGIC || b->tag <= 0)
    return NULL;
  return b;
}

static ZBlock *z_ring_next(Zone *z, ZBlock *b) {
  ZBlock *n = z_next(b);
  if (n->tag == ZT_SENTINEL) {
    ZChunk *c = z_sentinel_chunk(n);
    c = c->next ? c->next : z->chunks;
    return z_first(c);
  }
  return n;
}

static ZChunk *z_add_chunk(Zone *z, size_t need) {
  size_t min = Z_CHDR + need + Z_HDR;
  size_t bytes = z->chunk_size;
  ZChunk *c, **pp;
  ZBlock *b, *s;

  if (bytes < min)
    bytes = min;
  bytes = Z_ROUND(bytes);
  if (z->max_total && z->total + bytes > z->max_total) {
    bytes = Z_ROUND(min);
    if (z->total + bytes > z->max_total)
      return NULL;
  }
  c = (ZChunk *)malloc(bytes);
  if (!c)
    return NULL;
  c->next = NULL;
  c->bytes = bytes;

  b = z_first(c);
  b->size = bytes - Z_CHDR - Z_HDR;
  b->prev = NULL;
  b->user = NULL;
  b->tag = ZT_FREE;
  b->magic = ZMAGIC;

  s = z_next(b);
  s->size = Z_HDR;
  s->prev = b;
  s->tag = ZT_SENTINEL;
  s->magic = ZMAGIC;
  s->user = (void **)(void *)c;

  pp = &z->chunks;
  while (*pp)
    pp = &(*pp)->next;
  *pp = c;
  z->total += bytes;
  return c;
}

static void z_release_chunk(Zone *z, ZChunk *c) {
  ZChunk **pp = &z->chunks;
  while (*pp && *pp != c)
    pp = &(*pp)->next;
  if (!*pp)
    return;
  *pp = c->next;
  z->total -= c->bytes;
  z->rover = NULL;
  free(c);
}

static void z_maybe_release(Zone *z, ZBlock *b) {
  if (b->prev == NULL && z_next(b)->tag == ZT_SENTINEL) {
    ZChunk *c = z_chunk_of_first(b);
    if (c != z->chunks)
      z_release_chunk(z, c);
  }
}

static ZBlock *z_free_block(Zone *z, ZBlock *b) {
  ZBlock *p, *n;
  if (b->user)
    *b->user = NULL;
  b->user = NULL;
  z->used -= b->size;
  b->tag = ZT_FREE;

  p = b->prev;
  if (p && p->tag == ZT_FREE) {
    p->size += b->size;
    z_next(p)->prev = p;
    if (z->rover == b)
      z->rover = p;
    b->magic = 0;
    b = p;
  }
  n = z_next(b);
  if (n->tag == ZT_FREE) {
    b->size += n->size;
    z_next(b)->prev = b;
    if (z->rover == n)
      z->rover = b;
    n->magic = 0;
  }
  return b;
}

static ZBlock *z_find_free(Zone *z, size_t need) {
  ZBlock *start = z->rover ? z->rover : z_first(z->chunks);
  ZBlock *b = start;
  do {
    if (b->tag == ZT_FREE && b->size >= need)
      return b;
    b = z_ring_next(z, b);
  } while (b != start);
  return NULL;
}

static ZBlock *z_purge(Zone *z, size_t need) {
  ZChunk *c;
  for (c = z->chunks; c; c = c->next) {
    ZBlock *b = z_first(c);
    while (b->tag != ZT_SENTINEL) {
      if (b->tag >= ZONE_PURGABLE) {
        b = z_free_block(z, b);
        if (b->size >= need)
          return b;
      }
      b = z_next(b);
    }
  }
  return NULL;
}

static void *z_take(Zone *z, ZBlock *b, size_t need, int tag, void **user) {
  size_t extra = b->size - need;
  void *p;
  if (extra >= Z_MINBLOCK) {
    ZBlock *n = (ZBlock *)((char *)b + need);
    n->size = extra;
    n->prev = b;
    n->user = NULL;
    n->tag = ZT_FREE;
    n->magic = ZMAGIC;
    z_next(n)->prev = n;
    b->size = need;
  }
  b->tag = tag;
  b->user = user;
  z->used += b->size;
  z->rover = z_ring_next(z, b);
  p = (char *)b + Z_HDR;
  if (user)
    *user = p;
  return p;
}

// API

Zone *zone_create(size_t chunk_size, size_t max_total) {
  Zone *z;
  if (chunk_size < 4096)
    chunk_size = 4096;
  if (max_total && chunk_size > max_total)
    chunk_size = max_total;
  z = (Zone *)calloc(1, sizeof *z);
  if (!z)
    return NULL;
  z->chunk_size = Z_ROUND(chunk_size);
  z->max_total = max_total;
  if (!z_add_chunk(z, Z_MINBLOCK)) {
    free(z);
    return NULL;
  }
  z->rover = z_first(z->chunks);
  return z;
}

void zone_destroy(Zone *z) {
  ZChunk *c, *n;
  if (!z)
    return;
  for (c = z->chunks; c; c = n) {
    n = c->next;
    free(c);
  }
  free(z);
}

void *zone_alloc(Zone *z, size_t size, int tag, void **user) {
  size_t need;
  ZBlock *b;
  if (!z || tag <= 0)
    return NULL;
  if (tag >= ZONE_PURGABLE && !user)
    return NULL;
  if (size == 0)
    size = 1;
  if (size > SIZE_MAX / 4)
    return NULL;
  need = Z_ROUND(size + Z_HDR);

  b = z_find_free(z, need);
  if (!b) {
    ZChunk *c = z_add_chunk(z, need);
    if (c)
      b = z_first(c);
  }
  if (!b)
    b = z_purge(z, need);
  if (!b)
    return NULL;
  return z_take(z, b, need, tag, user);
}

void *zone_alloc_array(Zone *z, size_t count, size_t elem, int tag,
                       void **user) {
  size_t n;
  void *p;
  if (elem && count > SIZE_MAX / elem)
    return NULL;
  n = count * elem;
  p = zone_alloc(z, n, tag, user);
  if (p)
    memset(p, 0, n);
  return p;
}

void *zone_zalloc(Zone *z, size_t size, int tag, void **user) {
  return zone_alloc_array(z, 1, size, tag, user);
}

void *zone_realloc(Zone *z, void *p, size_t size) {
  ZBlock *b = z_header(p);
  size_t cap;
  void **u;
  int t;
  void *q;
  if (!z || !b)
    return NULL;
  cap = b->size - Z_HDR;
  if (size <= cap)
    return p;

  u = b->user;
  t = b->tag;
  b->user = NULL;
  b->tag = ZONE_STATIC;
  q = zone_alloc(z, size, t, u);
  if (!q) {
    b->user = u;
    b->tag = t;
    return NULL;
  }
  memcpy(q, p, cap);
  z_maybe_release(z, z_free_block(z, b));
  return q;
}

void zone_free(Zone *z, void *p) {
  ZBlock *b = z_header(p);
  if (!z || !b)
    return;
  z_maybe_release(z, z_free_block(z, b));
}

void zone_free_tags(Zone *z, int lo, int hi) {
  ZChunk *c, *nx;
  if (!z)
    return;
  for (c = z->chunks; c; c = c->next) {
    ZBlock *b = z_first(c);
    while (b->tag != ZT_SENTINEL) {
      if (b->tag > 0 && b->tag >= lo && b->tag <= hi)
        b = z_free_block(z, b);
      b = z_next(b);
    }
  }

  c = z->chunks ? z->chunks->next : NULL;
  while (c) {
    ZBlock *f = z_first(c);
    nx = c->next;
    if (f->tag == ZT_FREE && z_next(f)->tag == ZT_SENTINEL)
      z_release_chunk(z, c);
    c = nx;
  }
}

int zone_change_tag(void *p, int tag) {
  ZBlock *b = z_header(p);
  if (!b || tag <= 0)
    return 0;
  if (tag >= ZONE_PURGABLE && !b->user)
    return 0;
  b->tag = tag;
  return 1;
}

int zone_set_owner(void *p, void **user) {
  ZBlock *b = z_header(p);
  if (!b)
    return 0;
  if (!user && b->tag >= ZONE_PURGABLE)
    return 0;
  b->user = user;
  if (user)
    *user = p;
  return 1;
}

char *zone_strdup(Zone *z, const char *s, int tag, void **user) {
  size_t n = strlen(s) + 1;
  char *p = (char *)zone_alloc(z, n, tag, user);
  if (p)
    memcpy(p, s, n);
  return p;
}

char *zone_printf(Zone *z, int tag, void **user, const char *fmt, ...) {
  va_list ap, ap2;
  char *p = NULL;
  int n;
  va_start(ap, fmt);
  va_copy(ap2, ap);
  n = vsnprintf(NULL, 0, fmt, ap);
  va_end(ap);
  if (n >= 0) {
    p = (char *)zone_alloc(z, (size_t)n + 1, tag, user);
    if (p)
      vsnprintf(p, (size_t)n + 1, fmt, ap2);
  }
  va_end(ap2);
  return p;
}

size_t zone_used(const Zone *z) { return z->used; }
size_t zone_reserved(const Zone *z) { return z->total; }

const char *zone_check(const Zone *z) {
  size_t total = 0, used = 0;
  ZChunk *c;
  for (c = z->chunks; c; c = c->next) {
    ZBlock *b = z_first(c), *prev = NULL;
    char *sentinel_pos = (char *)c + c->bytes - Z_HDR;
    total += c->bytes;
    while (b->tag != ZT_SENTINEL) {
      ZBlock *n;
      if (b->magic != ZMAGIC)
        return "bad magic";
      if (b->prev != prev)
        return "bad prev link";
      if (b->size < Z_MINBLOCK || b->size % ZALIGN)
        return "bad block size";
      if ((char *)b + b->size > sentinel_pos)
        return "block overruns chunk";
      n = z_next(b);
      if (b->tag == ZT_FREE && n->tag == ZT_FREE)
        return "adjacent free blocks";
      if (b->tag >= ZONE_PURGABLE && !b->user)
        return "purgable block without owner";
      if (b->tag > 0)
        used += b->size;
      prev = b;
      b = n;
    }
    if ((char *)b != sentinel_pos)
      return "chunk size mismatch";
    if (b->magic != ZMAGIC || b->prev != prev)
      return "bad sentinel";
  }
  if (total != z->total)
    return "reserved total mismatch";
  if (used != z->used)
    return "used total mismatch";
  return NULL;
}

#endif /* ZONE_IMPLEMENTATION */
