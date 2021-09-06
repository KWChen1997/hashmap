#include <stddef.h>
#include <stdint.h>

#ifndef NIPQUAD
#define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]
#endif

#ifndef NIPQUAD_FMT
#define NIPQUAD_FMT "%u.%u.%u.%u"
#endif

#ifndef LOAD_FACTOR
#define LOAD_FACTOR 0.75
#endif

#ifndef IP2MAC
#define IP2MAC

struct ip2mac {
	uint32_t ip;			// key
	unsigned char mac[6];	// value
	char valid;
};

struct ip2mac_map {
	uint32_t cap;
	uint32_t count;
	uint32_t increm;
	struct ip2mac *list;
}__attribute__((packed));

int ip2mac_map_init(struct ip2mac_map *map, uint32_t increm);
uint32_t ip2mac_hash(uint32_t key, unsigned int cap);
struct ip2mac *ip2mac_find(struct ip2mac_map *map, uint32_t key);
int ip2mac_insert(struct ip2mac_map *map, struct ip2mac *entry);
int ip2mac_expand(struct ip2mac_map *map);

void ip2mac_print(struct ip2mac_map *map);

#endif
