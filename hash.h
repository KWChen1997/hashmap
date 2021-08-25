#include <stddef.h>
#include <stdint.h>

#ifndef HASH
#define HASH

#define DNS_MAX_LEN 255
#define LOAD_FACTOR 0.75

struct node {
	uint32_t key;
	void *nxt;
} __attribute((packed));

struct ip2dn {
	uint32_t ip;			// key
	void *nxt;
	char dn[DNS_MAX_LEN + 1];	// value
} __attribute__((packed));

struct map {
	uint32_t key;
	void *nxt;
	size_t entry_size;
	uint16_t increm;
	uint32_t cap;
	void **mapping;
	void *head;
	uint32_t count;
}__attribute__((packed));

struct stat {
	uint32_t port;
	void *nxt;
	int cnt;
	int intvl_cnt;
	int *intvl_cnts;
	uint16_t proto;
	float mean;
	float std_dev;
} __attribute__((packed));

int map_init(struct map *map, size_t entry_size, uint16_t increm);
uint32_t hash(uint32_t key, unsigned int cap); 
void **find(struct map *map, uint32_t key);
int insert(struct map *map, struct node *entry);
int expand(struct map *map);

void ip2dn_print(struct map *map);
void p2s_print(struct map *map);

#endif
