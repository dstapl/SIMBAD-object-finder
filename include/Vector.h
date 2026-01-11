/// Taken from https://www.youtube.com/watch?v=CwnPpX9Omu0
///	Author `Local`: [Abandoned] Creating a C Compiler from Scratch | Ep. 1
#ifndef _VECTOR_
#define _VECTOR_

/// Header data for a light-weight implelentation of typed vectors.
typedef struct {
    long long capacity;
    long long count;
} layec_vector_header;


void layec_vector_maybe_expand(void** vector_ref, long long element_size, long long required_count);

#define vector_get_header(V) (((layec_vector_header*)(V)) - 1)

void layec_vector_maybe_expand(void** vector_ref, long long element_size, long long required_count);

#define vector(T) T*
#define vector_count(V) ((V) ? vector_get_header(V)->count : 0)
#define vector_push(V, E) do { layec_vector_maybe_expand((void**)&(V), (long long)sizeof *(V), vector_count(V) + 1); (V)[vector_count(V)] = E; vector_get_header(V)->count++; } while (0)
#define vector_pop(V) do { if (vector_get_header(V)->count) vector_get_header(V)->count--; } while (0)
#define vector_free(V) do { if (V) { memset(V, 0, (unsigned long long)vector_count(V) * (sizeof *(V))); free(vector_get_header(V)); (V) = NULL; } } while (0)
#define vector_free_all(V, F) do { if (V) { for (long long vector_index = 0; vector_index < vector_count(V); vector_index++) F((V)[vector_index]); memset(V, 0, (unsigned long long)vector_count(V) * (sizeof *(V))); free(vector_get_header(V)); (V) = NULL; } } while (0)


#define vector_extend(V, ...) do { typeof(*(V)) temp_list[] = {__VA_ARGS__}; for (size_t i = 0; i < sizeof(temp_list)/sizeof(temp_list[0]); i++) { vector_push(V, temp_list[i]); }; } while (0)

#endif
