#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "hash.h"

/*
 * Calculate the hash value of the given key
 * parameters: 
 * 	key: target key value
 * 	cap: the bucket size of the hash map
 * return value: the hash value
 * */

uint32_t hash(uint32_t key, uint32_t cap){
	const uint32_t prime = 31;
	int i = 0;
	int l = sizeof(uint32_t);
	uint32_t hashval = 0;
	unsigned char *tmp = (unsigned char*) &key;
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

int map_init(struct map *map, size_t entry_size, uint16_t increm){
	map->cap = increm;
	map->mapping = (void**)malloc(sizeof(void*) * map->cap);
	if(map->mapping == NULL){
		perror("map_init 1");
		return -1;
	}
	map->head = NULL;
	map->count = 0;
	map->entry_size = entry_size;
	map->increm = increm;
	return 0;
}

/*
 * find the position where the item's address is stored at
 * return value: the address where stores the item's "address"
 * */

void **find(struct map *map, uint32_t key){
	uint32_t hashval = hash(key, map->cap);
	return map->mapping + hashval;
}

/*
 * insert a new entry into the map
 * */

int insert(struct map *map, struct node *entry){
	if((double)map->count/map->cap >= LOAD_FACTOR){
		if(expand(map) == -1){
			perror("expand");
			return -1;
		}
	}
	void **ptr = find(map, entry->key);
	if(*ptr == NULL){
		struct node *new = (struct node*)malloc(map->entry_size);
		memcpy(new, entry, map->entry_size);
		new->nxt = map->head;
		map->head = new;
		*ptr = new;
		map->count ++;
	}
	return 0;
}

/*
 * expand the bucket size of the map
 * return value: 0 for success, -1 for failure
 * */

int expand(struct map *map){
	uint32_t newcap = map->cap + map->increm;
	void **oldmapping = map->mapping;
	map->mapping = (void**)malloc(sizeof(void*) * newcap);
	if(map->mapping == NULL){
		perror("expand 1");
		return -1;
	}
	memset(map->mapping, 0, sizeof(void*) * newcap);

	struct node *cur = (struct node*)map->head;
	uint32_t hashval;
	while(cur != NULL){
		hashval = hash(cur->key, newcap);
		map->mapping[hashval] = cur;
		cur = cur->nxt;
	}

	int i = 0;
	if(oldmapping)
		free(oldmapping);
	
	map->cap = newcap;

	return 0;
}

void ip2dn_print(struct map *map){
	struct ip2dn *cur = (struct ip2dn*)map->head;
	while(cur != NULL){
		printf("key %u dn %s\n", cur->ip, cur->dn);
		cur = cur->nxt;
	}
	return;
}

void p2s_print(struct map *map){
	struct stat *cur = (struct stat*)map->head;
	while(cur != NULL){
		printf("port %u cnt %d intvl_cnt %d proto %u mean %f std_dev %f\n", cur->port, cur->cnt, cur->intvl_cnt, cur->proto, cur->mean, cur->std_dev);
		cur = cur->nxt;
	}
	return;
}
