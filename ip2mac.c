#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ip2mac.h"

void ip2mac_print(struct map *map){
	entry *cur;
	ip2mac *data;
	int i = 0;
	int j = 0;
	int k = 0;
	for(i = 0; i < map->cap; i++){
		if(map->list[i] == NULL)
			continue;
		cur = map->list[i];
		data = cur->data;
		printf("key %s mac %02x", cur->key, data->mac[0]);
		for(k = 1; k < 6; k++){
			printf(":%02x", data->mac[k]);
		}
		printf("\n");
		j++;
	}
	printf("count %d\n", j);
	return;
}
