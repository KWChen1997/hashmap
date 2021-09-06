#include <stdio.h>
#include "hash.h"
#include "ip2dn.h"

void ip2dn_print(struct map *map){
	entry *cur;
	ip2dn *data;
	int i = 0;
	int j = 0;
	for(i = 0; i < map->cap; i++){
		if(map->list[i] == NULL)
			continue;
		cur = map->list[i];
		data = cur->data;
		printf("key %s dn %s\n", cur->key, data->dn);
		j++;
	}
	printf("count %d\n", j);
	return;
}
