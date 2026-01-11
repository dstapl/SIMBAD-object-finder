#include <Map.h>
#include <Vector.h>


// Map

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
    // m.buckets = malloc(sizeof(void*)*m.cap);
	m.buckets = calloc(m.cap, sizeof(void*));
    assert((m.buckets != NULL) && "Get more RAM");
    return m;
}

int map_deinit(Map *m) {
	assert(m != NULL);
	if (!m) {
		perror("Pointer is NULL");
		return 1;
	}

	assert((m->buckets != NULL) && "Buckets are not allocated");
	if (m->buckets == NULL) {
		perror("Buckets are not allocated");
		return 1;
	}
	
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


void map_iter(Map *m, MapKVFn fn, void *extra_data) {
    for (size_t i = 0; i < m->cap; i++) {
        Map_KV *e = m->buckets[i];
        if (e) { // NULL check
            fn(e->key, e->value, extra_data);
        }
    }
}


// Vector
void layec_vector_maybe_expand(void** vector_ref, long long element_size, long long required_count)
{
    if (required_count <= 0) return;
    
    layec_vector_header* header = vector_get_header(*vector_ref);
    if (!*vector_ref)
    {
        long long initial_capacity = 32;
        void* new_data = malloc((sizeof *header) + (unsigned long long)(initial_capacity * element_size));
        header = (layec_vector_header*)new_data;

        header->capacity = initial_capacity;
        header->count = 0;

    }
    else if (required_count > header->capacity)
    {
        while (required_count > header->capacity)
            header->capacity *= 2;
        header = (layec_vector_header*)realloc(header, (sizeof *header) + (unsigned long long)(header->capacity * element_size));
    }
    
    *vector_ref = (void*)(header + 1);
}

