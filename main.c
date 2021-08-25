#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hash.h"


int main(){

	struct map map;
	map_init(&map);
	int i = 0;
	uint32_t ip;
	for(i = 0; i < 20; i++){
		ip = rand();
		insert(&map, &(struct entry){.key = ip, .dn = "localhost", .nxt = NULL});
	}

	printmap(&map);

	return 0;
}
