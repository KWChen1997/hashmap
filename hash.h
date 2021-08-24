#include <stddef.h>
#include <stdint.h>

#ifndef HASH
#define HASH

#define DNS_MAX_LEN 255
#define LOAD_FACTOR 0.75

struct entry {
	uint32_t ip;			// key
	char dn[DNS_MAX_LEN + 1];	// value
} __attribute__((packed));

struct map {
	uint32_t cap;
	char *occupied;
	struct entry *list;
	uint32_t count;
};

int map_init(struct map *map);

uint32_t hash1(uint32_t key, unsigned int cap); 
uint32_t find(struct map *map, uint32_t key);
uint32_t insert(struct map *map, struct entry *entry);
int expand(struct map *map);

#endif
