#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "hash.h"

#define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]
#define NIPQUAD_FMT "%u.%u.%u.%u"

// ip to domain name
typedef struct _ip2dn {
	char dn[256];
} ip2dn;

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

// domain name to statistic
typedef struct _intvl__node {
	int intvl_cnt;
	struct _intvl_node *next;
} intvl_node;

typedef struct _abtraf {
	int cnt;
	struct timeval t;
	struct _abtraf *next;
} abtraf;

typedef struct _dn2st {
	int cnt;
	int intvl_cnt;
	intvl_node *intvl_cnts;
	float mean;
	float std_dev;
	abtraf *ablist;
} dn2st;

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

// ip to mac
typedef struct _ip2mac {
	unsigned char mac[6];
} ip2mac;

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

// mac to dn2st map
typedef struct _mac2ds{
	struct map *dn2st;
} mac2ds;

void mac2ds_print(struct map *map){
	entry *cur;
	struct map *data;
	int i = 0;
	for(i = 0; i < map->cap; i++){
		if(map->list[i] == NULL)
			continue;
		data = map->list[i]->data;
		dn2st_print(data);
	}
	return;
}

int main(){

	uint32_t ip;
	int i = 0;
	/*
	struct map ip2dn_map;
	map_init(&ip2dn_map);
	char buf[16];
	
	for(i = 0; i < 1000; i++){
		entry e;
		ip2dn *tmp;
		ip = rand();
		e.key = (char*)malloc(sizeof(char) * 16);
		tmp = (ip2dn*)calloc(1,sizeof(ip2dn));

		snprintf(e.key,16,NIPQUAD_FMT,NIPQUAD(ip));
		e.data = tmp;
		
		strncpy(tmp->dn,"localhost",10);
		insert(&ip2dn_map, &e);
	}

	//ip2dn_print(&ip2dn_map);
	
	*/
	
	struct map dn2st_map[3];
	abtraf *abn;
	int n,j;
	int k;
	for(k = 0; k < 3; k++){
		map_init(&dn2st_map[k]);
		for(i = 0; i < 100; i++){
			entry e;
			e.key = (char*)malloc(sizeof(char)*(256+1+5+1+10));
			e.data = (dn2st*)calloc(1,sizeof(dn2st));
			dn2st *tmp = e.data;
	
			ip = rand();
			snprintf(e.key,273,"0x%08x-%d-%d", ip, rand() % 65536, 17);
			tmp->cnt = rand();
			tmp->intvl_cnt = rand();
			tmp->mean = 1.0 * rand() / rand();
			tmp->std_dev = 1.0 * rand() / rand();
			tmp->ablist = NULL;
			n = rand() % 4;
			for(j = 0; j < n; j++){
				abn = (abtraf*)malloc(sizeof(abtraf));
				abn->cnt = rand();
				gettimeofday(&abn->t,NULL);
				abn->next = tmp->ablist;
				tmp->ablist = abn;
			}
			insert(&dn2st_map[k], &e);
		}
		//printf("--------------------\n");
		//dn2st_print(&dn2st_map[k]);
	}

	struct map mac2ds_map;
	map_init(&mac2ds_map);
	
	for(i = 0; i < 3; i++){
		entry e;
		e.key = (char*)malloc(sizeof(char) * 18);
		e.data = &dn2st_map[i];
		snprintf(e.key, 18, "%02x:%02x:%02x:%02x:%02x:%02x", rand()%256,rand()%256,rand()%256,rand()%256,rand()%256,rand()%256);
		insert(&mac2ds_map, &e);
	}

	mac2ds_print(&mac2ds_map);
	/*
	struct map ip2mac_map;
	map_init(&ip2mac_map);
	int mac = 0;
	
	for(i = 0; i < 1000; i++){
		entry e;
		e.key = (char*)malloc(sizeof(char)*16);
		e.data = calloc(1,sizeof(ip2mac));
		ip2mac *tmp = e.data;
		ip = rand();
		mac = rand();
		snprintf(e.key,16,NIPQUAD_FMT,NIPQUAD(ip));
		memcpy(tmp->mac,&mac,6);
		insert(&ip2mac_map, &e);
	}
	ip2mac_print(&ip2mac_map);
	*/

/*
	struct ip2dn *res;
	while(scanf("%d", &ip) != 0){
		res = find(&ip2dn, ip);
		printf("ip %u.%u.%u.%u dn %s\n", NIPQUAD(res->ip), res->dn);
	}
*/
	return 0;
}
