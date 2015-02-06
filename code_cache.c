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

size_t size(Cache *cache) {
    return cache->size;
}

int is_empty(Cache *cache) {
    return 0 == size(cache);
}

int is_full(Cache *cache) {
    return size(cache) == cache->capacity;
}

static int find_entry(Cache *cache, const char *key) {
    if (is_full(cache)) {
        long lru = cache->counter;
        int retval = -1;
        for (int i = 0; i < capacity; ++i) {
            assert(cache->entries[i]);
            if (0 == strcmp(key, cache->entries[i]->key)) {
                retval = i;
                break;
            } else if (cache->entries[i]->last_access < lru) {
                lru = cache->entries[i]->last_access;
                retval = i;
            }
        }
        assert(retval != -1);
        assert(lru != cache->counter);
        cache->entries[retval]->last_access = cache->counter++;
        return retval;
    } else {
        for (int i = 0; i < capacity; ++i) {
            if (cache->entries[i] == NULL) {
                cache->entries[i] = (Entry *) malloc(sizeof(Entry));
                cache->entries->last_access = cache->counter++;
                cache->size++;
                return i;
            }
        }
        assert(0 && "unreachable");
        return -1;
    }
}

void *put(Cache *cache, const char *key, void *value) {
    const int i = find_entry(cache, key);
    cache->entries[i].
}

void *get(Cache *cache, const char *key) {
}
