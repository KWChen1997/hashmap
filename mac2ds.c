#include <stdio.h>
#include "hash.h"
#include "mac2ds.h"

void mac2ds_print(struct map *map){
	struct map *data;
	int i = 0;
	for(i = 0; i < map->cap; i++){
		if(map->list[i] == NULL)
			continue;
		data = map->list[i]->data;
		data->printmap(data);
	}
	return;
}
