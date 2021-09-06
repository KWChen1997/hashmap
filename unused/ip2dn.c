#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "ip2dn.h"

/*
 * Calculate the hash value of the given key
 * parameters: 
 * 	key: target key value
 * 	cap: the bucket size of the hash map
 * return value: the hash value
 * */

uint32_t ip2dn_hash(uint32_t key, uint32_t cap){
	const uint32_t prime = 31;
	int i = 0;
	int l = sizeof(uint32_t);
	uint32_t hashval = 0;
	uint8_t *tmp = (uint8_t*) &key;
	for(i = 0; i < l; i++){
		hashval = (hashval * prime + *(tmp + i)) % cap;
	}

	return hashval;
}

/*
 * Initialize the map data structure
 * parameters:
 * 	map: the address of the map that need to be initialized
 * 	entry_size: the entry size of the stored item
 * 	increm: the expand bucket size when the load factor exceed the threshold
 * return value: 0 for success, -1 for failure
 * */

int ip2dn_map_init(struct ip2dn_map *map, uint32_t increm){
	map->cap = increm;
	map->count = 0;
	map->increm = increm;
	map->list = (struct ip2dn*)malloc(sizeof(struct ip2dn) * map->cap);
	if(map->list == NULL){
		perror("map init malloc");
		return -1;
	}
	memset(map->list, 0, sizeof(struct ip2dn) * map->cap);
	return 0;
}

/*
 * find the position where the item's address is stored at
 * return value: the address where stores the item's "address"
 * */

struct ip2dn *ip2dn_find(struct ip2dn_map *map, uint32_t key){
	uint32_t hashval = ip2dn_hash(key, map->cap);
	struct ip2dn *res = map->list + hashval;
	while(res->valid && res->ip != key){
		hashval = (hashval + 1) % map->cap;
		res = map->list + hashval;
	}
	return res;
}

/*
 * insert a new entry into the map
 * */

int ip2dn_insert(struct ip2dn_map *map, struct ip2dn *entry){
	if((double)map->count/map->cap >= LOAD_FACTOR){
		if(ip2dn_expand(map) == -1){
			perror("ip2dn_expand");
			return -1;
		}
	}

	struct ip2dn *res = ip2dn_find(map, entry->ip);
	memcpy(res, entry, sizeof(struct ip2dn));
	res->valid = 1;
	map->count ++;
	return 0;
}

/*
 * expand the bucket size of the map
 * return value: 0 for success, -1 for failure
 * */

int ip2dn_expand(struct ip2dn_map *map){
	struct ip2dn *oldlist = map->list;
	uint32_t oldcap = map->cap;
	map->cap = map->cap + map->increm;
	map->list = (struct ip2dn*)malloc(sizeof(struct ip2dn) * map->cap);
	if(map->list == NULL){
		perror("malloc");
		return -1;
	}
	memset(map->list, 0, sizeof(struct ip2dn) * map->cap);
	map->count = 0;
	int i = 0;
	for(i = 0; oldlist != NULL && i < oldcap; i++){
		if(oldlist[i].valid)
			ip2dn_insert(map,oldlist + i);
	}

	if(oldlist)
		free(oldlist);

	return 0;
}

void ip2dn_print(struct ip2dn_map *map){
	int i = 0;
	int j = 0;
	for(i = 0; i < map->cap; i++){
		if(map->list[i].valid){
			printf("ip %u.%u.%u.%u dn %s\n", NIPQUAD(map->list[i].ip), map->list[i].dn);
			j++;
		}
	}
	printf("count %d\n",j);
	return;
}

