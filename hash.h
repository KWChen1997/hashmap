#include <stddef.h>
#include <stdint.h>

#ifndef HASH
#define HASH

#define DNS_MAX_LEN 255
#define LOAD_FACTOR 0.75


typedef struct _entry {
	char *key;			// key
	void *data;
} entry;

typedef struct _node {
	entry e;
	struct _node *next;
} node;

struct map {
	entry **list;
	node *head;
	uint32_t cap;
	uint32_t count;
	void (*printmap)(struct map*);
};

typedef void (*printmap)(struct map*);

int map_init(struct map *map,printmap p);
uint32_t hash(char *key, unsigned int cap); 
entry *find(struct map *map, char *key);
int insert(struct map *map, entry *entry);
int expand(struct map *map);

#endif
