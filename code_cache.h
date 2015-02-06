#ifndef _CODE_CACHE_H_
#define _CODE_CACHE_H_

struct code_cache;
typedef struct code_cache Cache;

Cache *new_cache(size_t capacity);
size_t size(Cache *cache);
int is_empty(Cache *cache);
int is_full(Cache *cache);

void *put(Cache *cache, const char *key, void *value);
void *get(Cache *cache, const char *key);

#endif
