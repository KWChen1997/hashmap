#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "hash.h"

#ifndef DN2ST
#define DN2ST

typedef struct _intvl__node {
	int intvl_cnt;
	struct _intvl_node *next;
} intvl_node;

typedef struct _abtraf {
	int cnt;
	struct timeval t;
	struct _abtraf *next;
} abtraf;

typedef struct _dn2st {
	int cnt;
	int intvl_cnt;
	intvl_node *intvl_cnts;
	float mean;
	float std_dev;
	abtraf *ablist;
} dn2st;

void dn2st_print(struct map *map);

#endif
