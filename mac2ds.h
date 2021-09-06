#include "hash.h"

#ifndef MAC2DS
#define MAC2DS

typedef struct _mac2ds{
	struct map *dn2st;
} mac2ds;

void mac2ds_print(struct map *map);

#endif
