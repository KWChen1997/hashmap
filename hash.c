#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "hash.h"

#define BUCKETINCREM 100

uint32_t hash1(uint32_t key, uint32_t cap){
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
	map->occupied = (char*)malloc(sizeof(char) * BUCKETINCREM);
	if(map->occupied == NULL){
		perror("Map init malloc 1");
		return -1;
	}
	memset(map->occupied,0,sizeof(char) * BUCKETINCREM);
	map->list = (struct entry*)malloc(sizeof(struct entry) * BUCKETINCREM);
	if(map->list == NULL){
		perror("Map init malloc 2");
		return -1;
	}
	memset(map->list,0,sizeof(struct entry) * BUCKETINCREM);
	map->count = 0;
	map->cap = BUCKETINCREM;
	return 0;
}

uint32_t find(struct map *map, uint32_t key){
	uint32_t hashval = hash1(key,map->cap);
	while(map->occupied[hashval] && map->list[hashval].ip != key){
		// printf("Collision %u\n", hashval);
		hashval = (hashval + 1) % map->cap;
	}

	return hashval;
}

uint32_t insert(struct map *map, struct entry *entry){
	if((double)map->count/map->cap >= LOAD_FACTOR){
		// printf("Map expansion\n");
		if(expand(map) == -1){
			perror("expand");
			return -1;
		}
	}
	unsigned hashval = find(map, entry->ip);
	
	// Miss
	if(!map->occupied[hashval]){
		// printf("Insert %x at %u\n", entry->ip, hashval);
		memcpy(map->list + hashval, entry, sizeof(struct entry));
		map->occupied[hashval] = 1;
		map->count++;
	}	
	return hashval;	
}

int expand(struct map *map){
	int i = 0;
	uint32_t oldcap = map->cap;
	char *oldocc = map->occupied;
	map->cap += BUCKETINCREM;

	struct entry *oldlist = map->list;
	map->list = (struct entry*)malloc(sizeof(struct entry) * map->cap);
	if(map->list == NULL){
		perror("expand malloc 1");
		return -1;
	}
	memset(map->list, 0, sizeof(struct entry) * map->cap);

	map->occupied = (char*)malloc(sizeof(char) * map->cap);
	if(map->occupied == NULL){
		perror("expand malloc 2");
		return -1;
	}
	memset(map->occupied, 0, sizeof(char) * map->cap);

	map->count = 0;
	for(i = 0; i < oldcap; i++){
		if(!oldocc[i])
			continue;
		insert(map, oldlist + i);
	}
	
	if(oldlist)
		free(oldlist);
	if(oldocc)
		free(oldocc);

	return 0;
}

void printmap(struct map *map){
	int i = 0;
	for(i = 0; i < map->cap; i++){
		if(!map->occupied[i])
			continue;
		printf("%d ip 0x%x dn %s\n", i, map->list[i].ip, map->list[i].dn);
	}
	return;
}
