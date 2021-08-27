#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include "ip2dn.h"


int main(){

	struct map ip2dn;
	map_init(&ip2dn, 100);
	int i = 0;
	uint32_t ip;
	char buf[16];
	struct ip2dn tmp;
	for(i = 0; i < 10000; i++){
		ip = rand();
		snprintf(buf,16,"0x%x",ip);
		memset(&tmp, 0, sizeof(struct ip2dn));
		tmp.ip = ip;
		strncpy(tmp.dn,buf,16);
		insert(&ip2dn, &tmp);
	}
	ip2dn_print(&ip2dn);
	
/*
	struct ip2dn *res;
	while(scanf("%d", &ip) != 0){
		res = find(&ip2dn, ip);
		printf("ip %u.%u.%u.%u dn %s\n", NIPQUAD(res->ip), res->dn);
	}
*/
	return 0;
}
