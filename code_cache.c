#include "code_cache.h"

struct entry {
    char *key;
    long last_access;
    void *value;
};

typedef struct entry Entry;

struct code_cache {
    long counter;
    size_t size;
    size_t capacity;
    Entry **entries;
};

Cache *new_cache(size_t capacity) {
    Cache *cache = (Cache *) malloc(sizeof(Cache));
    cache->entries = (Entry **) malloc(sizeof(Entry *) * capacity);
    for (int i = 0; i < capacity; ++i) {
        cache->entries[i] = NULL;
    }
    cache->capacity = capacity;
    cache->size = 0;
    cache->counter = 0;
    return cache;
}

size_t size(const Cache *cache) {
    assert(cache);
    return cache->size;
}

size_t capacity(const Cache *cache) {
    assert(cache);
    return cache->capacity;
}

int is_empty(const Cache *cache) {
    return 0 == size(cache);
}

int is_full(const Cache *cache) {
    return size(cache) == cache->capacity;
}

Entry *find_entry(Cache *cache, const char *key, int create) {
    int lru_i = -1;
    long lru = cache->counter; /* guanranteed to be biggest */

    for (int i = 0; i < capacity(cache); ++i) {
        if (cache->entries[i] == NULL) {
            lru_i = i;
        } else if (0 == strcmp(key, cache->entries[i]->key)) {
            /* if found, early return */
            cache->entries[i]->last_access = cache->counter++;
            return cache->entries[i];
        } else if (lru_i == -1 || (cache->entries[lru_i] != NULL &&
                                   cache->entries[i]->last_access < lru)) {
            lru = cache->entries[i]->last_access;
            lru_i = i;
        }
    }

    assert(cache->entries[lru_i] == NULL ||
           cache->entries[lru_i]->last_access < cache->counter);

    /* not found, create or update */
    if (cache->entries[lru_i] == NULL) {
        if (create) {
            cache->entries[lru_i] = (Entry *) malloc(sizeof(Entry));
            cache->entries[lru_i]->key = NULL;
            cache->entries[lru_i]->value = NULL;
        } else {
            return NULL;
        }
    }

    Entry *entry = cache->entries[lru_i];
    entry->key = (char *) realloc(entry->key, sizeof(char) * (strlen(key) + 1));
    strcpy(entry->key, key);
    entry->last_access = cache->counter++;
    return entry;
}

void *put(Cache *cache, const char *key, void *value) {
    Entry *entry = find_entry(cache, key, 1);
    void *oldptr = entry->value;
    entry->value = value;
    return oldptr;
}

void *get(Cache *cache, const char *key) {
    const Entry *entry = find_entry(cache, key, 0);
    if (entry == NULL) {
        return NULL;
    } else {
        return entry->value;
    }
}
