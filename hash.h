#include <stddef.h>
#include <stdint.h>

#ifndef HASH
#define HASH

#define DNS_MAX_LEN 255
#define LOAD_FACTOR 0.75

struct entry {
	uint32_t key;			// key
	char dn[DNS_MAX_LEN + 1];	// value
	struct entry *nxt;
} __attribute__((packed));

struct map {
	uint32_t cap;
	void **mapping;
	void *head;
	uint32_t count;
};

int map_init(struct map *map);
uint32_t hash(uint32_t key, unsigned int cap); 
void **find(struct map *map, uint32_t key);
int insert(struct map *map, struct entry *entry);
int expand(struct map *map);

void printmap(struct map *map);

#endif
