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

#ifndef DNS_MAX_LEN
#define DNS_MAX_LEN 255
#endif

#ifndef LOAD_FACTOR
#define LOAD_FACTOR 0.75
#endif

#ifndef IP2DN
#define IP2DN

struct ip2dn {
	uint32_t ip;			// key
	char dn[DNS_MAX_LEN + 1];	// value
	char valid;
} __attribute__((packed));

struct map {
	uint32_t cap;
	uint32_t count;
	uint32_t increm;
	struct ip2dn *list;
}__attribute__((packed));

int map_init(struct map *map, uint32_t increm);
uint32_t hash(uint32_t key, unsigned int cap);
struct ip2dn *find(struct map *map, uint32_t key);
int insert(struct map *map, struct ip2dn *entry);
int expand(struct map *map);

void ip2dn_print(struct map *map);

#endif
