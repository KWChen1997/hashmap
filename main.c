#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include "ip2dn.h"
#include "dn2st.h"


int main(){
	struct ip2dn_map ip2dn;
	ip2dn_map_init(&ip2dn, 5);
	int i = 0;
	uint32_t ip;
	char buf[16];
	/*
	for(i = 0; i < 1000; i++){
		struct ip2dn tmp;
		ip = rand();
		snprintf(buf,16,"0x%x",ip);
		memset(&tmp, 0, sizeof(struct ip2dn));
		tmp.ip = ip;
		strncpy(tmp.dn,buf,16);
		ip2dn_insert(&ip2dn, &tmp);
	}
	ip2dn_print(&ip2dn);
	*/
	struct dn2st_map dn2st;
	struct abntraf *abn;
	int n,j;
	dn2st_map_init(&dn2st, 5);
	for(i = 0; i < 20; i++){
		struct dn2st tmp;
		ip = rand();
		snprintf(buf,16,"0x%x",ip);
		memset(&tmp, 0, sizeof(struct dn2st));
		strncpy(tmp.key.dn,buf,16);
		tmp.key.port = rand() % 65536;
		tmp.key.proto = 17;
		tmp.cnt = rand();
		tmp.intvl_cnt = rand();
		tmp.mean = 1.0 * rand() / rand();
		tmp.std_dev = 1.0 * rand() / rand();
		tmp.ablist = NULL;
		n = rand() % 4;
		for(j = 0; j < n; j++){
			abn = (struct abntraf*)malloc(sizeof(struct abntraf));
			abn->cnt = rand();
			gettimeofday(&abn->t,NULL);
			abn->nxt = tmp.ablist;
			tmp.ablist = abn;
		}
		dn2st_insert(&dn2st, &tmp);
	}

	dn2st_print(&dn2st);
	
	
/*
	struct ip2dn *res;
	while(scanf("%d", &ip) != 0){
		res = find(&ip2dn, ip);
		printf("ip %u.%u.%u.%u dn %s\n", NIPQUAD(res->ip), res->dn);
	}
*/
	return 0;
}
