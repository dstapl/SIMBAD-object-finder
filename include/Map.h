/// Adapted from xnacly: https://xnacly.me/posts/2024/c-hash-map/
/// Currently does NOT include resizing when size out grows capacity
#ifndef _HASHMAP_
#define _HASHMAP_


#include <stdlib.h>
#include <assert.h>

typedef struct { size_t size; size_t cap; void **buckets; } Map;

// FNV1a
extern const size_t BASE;
extern const size_t PRIME;
size_t map_key_hash(Map *m, const char *str);

Map map_init(size_t cap);

int map_deinit(Map *m);

void map_put(Map *m, const char *str, void *value);

void* map_get(Map *m, const char *str);


void* map_get_safe(Map *m, const char *str);


typedef struct {
    void *key;
    void *value;
} Map_KV;

typedef void (*MapKVFn)(const void *key, void *value, void *extra);

// Pass in extra data from the call site if needed
void map_iter(Map *m, MapKVFn fn, void *extra);

#endif
