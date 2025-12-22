/// Adapted from xnacly: https://xnacly.me/posts/2024/c-hash-map/
/// Currently does NOT include resizing when size out grows capacity
#ifndef _HASHMAP_
#define _HASHMAP_

#include <stdlib.h>
#include <assert.h>

typedef struct { size_t size; size_t cap; void **buckets; } Map;

// FNV1a
const size_t BASE = 0x811c9dc5;
const size_t PRIME = 0x01000193;
size_t map_key_hash(Map *m, const char *str) {
    size_t initial = BASE;
    while(*str) {
        initial ^= *str++;
        initial *= PRIME;
    }
    return initial & (m->cap - 1);
}

Map map_init(size_t cap) {
    Map m = {0,cap, NULL};
    m.buckets = malloc(sizeof(void*)*m.cap);
    assert((m.buckets != NULL) && "Get more RAM");
    return m;
}

int map_deinit(Map *m) {
	assert(m != NULL);
	assert((m->buckets != NULL) && "Buckets are not allocated");
	
	free(m->buckets);
	m->buckets = NULL;
	m->size = 0;
	m->cap = 0;

	return 0;
}

void map_put(Map *m, const char *str, void *value) {
    m->size++;
    m->buckets[map_key_hash(m, str)] = value;
}

void* map_get(Map *m, const char *str) {
	assert((m->buckets != NULL) && "Buckets are not allocated");
    return m->buckets[map_key_hash(m, str)];
}


void* map_get_safe(Map *m, const char *str) {
	if (m->buckets == NULL) {
		return NULL;
	}

	return map_get(m, str);
}

#endif
