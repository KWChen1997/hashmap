#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <time.h>
#include "dn2st.h"

/*
 * Calculate the hash value of the given key
 * parameters: 
 * 	key: target key value
 * 	cap: the bucket size of the hash map
 * return value: the hash value
 * */

uint32_t dn2st_hash(struct key *key, uint32_t cap){
	const uint32_t prime = 31;
	int i = 0;
	int l = sizeof(struct key);
	uint32_t hashval = 0;
	uint8_t *tmp = (uint8_t*) key;
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

int dn2st_map_init(struct dn2st_map *map, uint32_t increm){
	map->cap = increm;
	map->count = 0;
	map->increm = increm;
	map->list = (struct dn2st*)malloc(sizeof(struct dn2st) * map->cap);
	if(map->list == NULL){
		perror("map init malloc");
		return -1;
	}
	memset(map->list, 0, sizeof(struct dn2st) * map->cap);
	return 0;
}

/*
 * find the position where the item's address is stored at
 * return value: the address where stores the item's "address"
 * */

struct dn2st *dn2st_find(struct dn2st_map *map, struct key *key){
	uint32_t hashval = dn2st_hash(key, map->cap);
	struct dn2st *res = map->list + hashval;
	while(res->valid && !(res->key.port == key->port && res->key.proto == key->proto && strcmp(res->key.dn, key->dn) == 0)){
		hashval = (hashval + 1) % map->cap;
		res = map->list + hashval;
	}
	return res;
}

/*
 * insert a new entry into the map
 * */

int dn2st_insert(struct dn2st_map *map, struct dn2st *entry){
	if((double)map->count/map->cap >= LOAD_FACTOR){
		if(dn2st_expand(map) == -1){
			perror("expand");
			return -1;
		}
	}

	struct dn2st *res = dn2st_find(map, &entry->key);
	memcpy(res, entry, sizeof(struct dn2st));
	res->valid = 1;
	map->count ++;
	return 0;
}

/*
 * expand the bucket size of the map
 * return value: 0 for success, -1 for failure
 * */

int dn2st_expand(struct dn2st_map *map){
	struct dn2st *oldlist = map->list;
	uint32_t oldcap = map->cap;
	map->cap = map->cap + map->increm;
	map->list = (struct dn2st*)malloc(sizeof(struct dn2st) * map->cap);
	if(map->list == NULL){
		perror("malloc");
		return -1;
	}
	memset(map->list, 0, sizeof(struct dn2st) * map->cap);
	map->count = 0;
	int i = 0;
	for(i = 0; oldlist != NULL && i < oldcap; i++){
		if(oldlist[i].valid)
			dn2st_insert(map,oldlist + i);
	}

	if(oldlist)
		free(oldlist);

	return 0;
}

void dn2st_print(struct dn2st_map *map){
	int i = 0;
	int j = 0;
	for(i = 0; i < map->cap; i++){
		if(map->list[i].valid){
			printf("dn %s port %d proto %d cnt %d intvl_cnt %d mean %f std_dev %f\n", map->list[i].key.dn, map->list[i].key.port, map->list[i].key.proto, map->list[i].cnt, map->list[i].intvl_cnt, map->list[i].mean, map->list[i].std_dev);
			struct abntraf *cur = map->list[i].ablist;
			while(cur != NULL){
				printf("cnt %d time %s", cur->cnt, asctime(gmtime(&cur->t.tv_sec)));
				cur = cur->nxt;
			}	
			j++;
		}
	}
	printf("count %d\n",j);
	return;
}

