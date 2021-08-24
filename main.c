#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hash.h"


int main(){

	struct map map;
	map_init(&map);
	int i = 0;
	uint32_t hashval;
	for(i = 0; i < 20; i++){
		hashval = insert(&map, &(struct entry){.ip = rand(), .dn = "localhost"});
	}

	return 0;
}
