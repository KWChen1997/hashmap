#include <stdio.h>
#include "hash.h"
#include "dn2st.h"

void dn2st_print(struct map *map){
	entry *cur;
	dn2st *data;
	int i = 0;
	int j = 0;
	for(i = 0; i < map->cap; i++){
		if(map->list[i] == NULL)
			continue;
		cur = map->list[i];
		data = cur->data;
		printf("key %s cnt %d intvl_cnt %d mean %f std_dev %f\n", cur->key, data->cnt, data->intvl_cnt, data->mean, data->std_dev);
		abtraf *tmp = data->ablist;
		while(tmp != NULL){
			printf("\tcnt %d time %s", tmp->cnt, asctime(gmtime(&tmp->t.tv_sec)));
			tmp = tmp->next;
		}
		j++;
	}
	printf("count %d\n",j);
}
