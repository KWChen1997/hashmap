#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include "hash.h"
#include "ip2dn.h"
#include "dn2st.h"
#include "ip2mac.h"
#include "mac2ds.h"

#define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]
#define NIPQUAD_FMT "%u.%u.%u.%u"

int main(){

	uint32_t ip;
	int i = 0;
	
	struct map ip2dn_map;
	map_init(&ip2dn_map, &ip2dn_print);
	
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

	ip2dn_map.printmap(&ip2dn_map);
	
	struct map dn2st_map[3];
	abtraf *abn;
	int n,j;
	int k;
	for(k = 0; k < 3; k++){
		map_init(&dn2st_map[k], &dn2st_print);
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
		printf("--------------------\n");
		dn2st_map[k].printmap(&dn2st_map[k]);
	}
	
	
	struct map mac2ds_map;
	map_init(&mac2ds_map, &mac2ds_print);
	
	for(i = 0; i < 3; i++){
		entry e;
		e.key = (char*)malloc(sizeof(char) * 18);
		e.data = &dn2st_map[i];
		snprintf(e.key, 18, "%02x:%02x:%02x:%02x:%02x:%02x", rand()%256,rand()%256,rand()%256,rand()%256,rand()%256,rand()%256);
		insert(&mac2ds_map, &e);
	}

	mac2ds_map.printmap(&mac2ds_map);
	
	
	struct map ip2mac_map;
	map_init(&ip2mac_map, &ip2mac_print);
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
	ip2mac_map.printmap(&ip2mac_map);
	

/*
	struct ip2dn *res;
	while(scanf("%d", &ip) != 0){
		res = find(&ip2dn, ip);
		printf("ip %u.%u.%u.%u dn %s\n", NIPQUAD(res->ip), res->dn);
	}
*/
	return 0;
}
