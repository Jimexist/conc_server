#ifndef _CODE_CACHE_H_
#define _CODE_CACHE_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct code_cache;
typedef struct code_cache Cache;

Cache *new_cache(size_t capacity);
size_t size(const Cache *cache);
int is_empty(const Cache *cache);
int is_full(const Cache *cache);

void *put(Cache *cache, const char *key, void *value);
void *get(Cache *cache, const char *key);

#endif
