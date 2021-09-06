#include "hash.h"

#ifndef IP2MAC
#define IP2MAC

typedef struct _ip2mac {
	unsigned char mac[6];
} ip2mac;

void ip2mac_print(struct map *map);

#endif
