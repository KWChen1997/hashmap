#include <stddef.h>
#include <stdint.h>
#include "hash.h"

#ifndef IP2DN
#define IP2DN

typedef struct _ip2dn {
	char dn[256];
} ip2dn;

void ip2dn_print(struct map *map);

#endif
