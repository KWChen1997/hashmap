#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hash.h"


int main(){

	struct map ip2dn;
	struct map p2s;
	map_init(&ip2dn, sizeof(struct ip2dn), 10);
	map_init(&p2s, sizeof(struct stat), 10);
	int i = 0;
	uint32_t ip;
	char buf[16];
	struct ip2dn tmp;
	for(i = 0; i < 20; i++){
		ip = rand();
		snprintf(buf,16,"0x%x",ip);
		tmp.ip = ip;
		strncpy(tmp.dn,buf,16);
		tmp.nxt = NULL;
		insert(&ip2dn, (struct node*)&tmp);
	}

	ip2dn_print(&ip2dn);

	struct stat tmpstat;

	for(i = 0; i < 20; i++){
		tmpstat.port = i + 1;
		tmpstat.nxt = NULL;
		tmpstat.cnt = rand();
		tmpstat.intvl_cnt = rand();
		tmpstat.intvl_cnts = NULL;
		tmpstat.proto = 17;
		tmpstat.mean = 1.0 * rand() / rand();
		tmpstat.std_dev = 1.0 * rand() / rand();
		insert(&p2s, (struct node*)&tmpstat);
	}

	p2s_print(&p2s);

	struct ip2dn *ptr;
	while(scanf("%u", &ip) != 0){
		ptr = *find(&ip2dn, ip);
		if(ptr == NULL || ptr->ip != ip)
			printf("Miss\n");
		else
			printf("key %u dn %s\n", ptr->ip, ptr->dn);
	}
	return 0;
}
