#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include "hash.h"

#define INCREM 100

uint32_t hash(char *key, uint32_t cap){
	uint32_t hashval = 0;
	char *c = key;
	for(c = key; *c != '\0'; c++){
		hashval = (hashval * 31 + *c) % cap;
	}

	return hashval;
}

int map_init(struct map *map){
	map->list = (entry**)calloc(INCREM,sizeof(entry*));
	map->head = NULL; 
	map->cap = INCREM;
	map->count = 0;
	return 0;
}

entry *find(struct map *map, char *key){
	uint32_t hashval = hash(key, map->cap);
	while(map->list[hashval] != NULL && strcmp(map->list[hashval]->key,key) != 0)
		hashval = (hashval + 1) % map->cap;

	return map->list[hashval];
}

int insert(struct map *map, entry *entry){
	if(1.0*map->count/map->cap > LOAD_FACTOR)
		expand(map);
	uint32_t hashval = hash(entry->key, map->cap);
	while(map->list[hashval] != NULL && strcmp(map->list[hashval]->key,entry->key) != 0)
		hashval = (hashval + 1) % map->cap;
	if(map->list[hashval] != NULL)
		return 0;
	node *new = (node*)malloc(sizeof(node));
	new->e.key = entry->key;
	new->e.data = entry->data;
	new->next = map->head;
	map->head = new;
	map->list[hashval] = &new->e;
	map->count++;
	return 0;
}

int expand(struct map *map){
	uint32_t oldcap = map->cap;
	map->cap += INCREM;
	entry **oldlist = map->list;
	map->list = (entry**)calloc(map->cap, sizeof(entry*));
	if(map->list == NULL){
		perror("calloc");
		return -1;
	}

	node *cur = map->head;
	while(cur != NULL){
		uint32_t hashval = hash(cur->e.key, map->cap);
		while(map->list[hashval] != NULL)
			hashval = (hashval + 1) % map->cap;
		map->list[hashval] = &cur->e;
		cur = cur->next;
	}

	if(oldlist)
		free(oldlist);

	return 0;
}

void printmap(struct map *map){
	entry *cur;
	struct {
		uint32_t ip;
		char dn[256];
	} *tmp;
	int i = 0;
	int j = 0;
	for(i = 0; i < map->cap; i++){
		if(map->list[i] == NULL)
			continue;
		cur = map->list[i];
		tmp = cur->data;
		printf("key %s data ip 0x%x dn %s\n", cur->key, tmp->ip, tmp->dn);
		j++;
	}
	printf("count %d\n", j);
	return;
}
