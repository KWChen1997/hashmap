#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "hash.h"

#define BUCKETINCREM 100

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

int map_init(struct map *map){
	map->cap += BUCKETINCREM;
	map->mapping = (void**)malloc(sizeof(void*) * map->cap);
	if(map->mapping == NULL){
		perror("map_init 1");
		return -1;
	}
	map->head = NULL;
	map->count = 0;

	return 0;
}

void **find(struct map *map, uint32_t key){
	uint32_t hashval = hash(key, map->cap);
	return map->mapping + hashval;
}

int insert(struct map *map, struct entry *entry){
	if((double)map->count/map->cap >= LOAD_FACTOR){
		if(expand(map) == -1){
			perror("expand");
			return -1;
		}
	}
	void **ptr = find(map, entry->key);
	if(*ptr == NULL){
		struct entry *new = (struct entry*)malloc(sizeof(struct entry));
		memcpy(new, entry, sizeof(struct entry));
		new->nxt = (struct entry*)map->head;
		map->head = (void*)new;
		*ptr = new;
		map->count ++;
	}
	return 0;
}

int expand(struct map *map){
	uint32_t newcap = map->cap + BUCKETINCREM;
	void **oldmapping = map->mapping;
	map->mapping = (void**)malloc(sizeof(void*) * newcap);
	if(map->mapping == NULL){
		perror("expand 1");
		return -1;
	}
	memset(map->mapping, 0, sizeof(void*) * newcap);

	struct entry *cur = (struct entry*)map->head;
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

void printmap(struct map *map){
	struct entry *cur = (struct entry*)map->head;
	while(cur != NULL){
		printf("key 0x%x dn %s\n", cur->key, cur->dn);
		cur = cur->nxt;
	}
	return;
}
