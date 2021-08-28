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

#ifndef DN2ST
#define DN2ST

struct node {
	int intvl_cnt;
	struct node *next;
};

struct key {
	char dn[DNS_MAX_LEN + 1];
	int port;
	int proto;
};

struct dn2st {
	struct key key;
	int cnt;
	int intvl_cnt;
	struct node *intvl_cnts;
	float mean;
	float std_dev;
	char valid;
};

struct dn2st_map {
	uint32_t cap;
	uint32_t count;
	uint32_t increm;
	struct dn2st *list;
}__attribute__((packed));

int dn2st_map_init(struct dn2st_map *map, uint32_t increm);
uint32_t dn2st_hash(struct key *key, unsigned int cap);
struct dn2st *dn2st_find(struct dn2st_map *map, struct key *key);
int dn2st_insert(struct dn2st_map *map, struct dn2st *entry);
int dn2st_expand(struct dn2st_map *map);

void dn2st_print(struct dn2st_map *map);

#endif
