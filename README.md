# Simple Hash Map for ip/dn mapping

## Entry data structure
```sh=
struct ip2dn {
	uint32_t ip;
	char dn[DNS_MAX_LEN + 1];
	char valid;
};
```
- please make sure memcpy will deep copy of your entry

## ip2dn Functions
- int map_init(struct map *map, uint16_t increm);
	- Initialize the map data structure
	- Return Value
		- If success, 0 will be returned. If failed, 1 will be returned.
- int hash(uint32_t key, unsigned int cap); 
	- Calculate the hash value according to the given key which is ip in these design
	- Return value
		- The hash value corresponding to the key.
- struct ip2dn *find(struct map *map, uint32_t key);
	- Find the empty slot or the slot of the key
	- Return Value
		- If there is a hit, the return pointer to the entry.
- uint32_t insert(struct map *map, struct ip2dn *entry);
	- Insert the entry into the map
	- Return value
		- If success, 0 will be returned. If failed, 1 will be returned.
- int expand(struct map *map);
	- expand the bucket size of the map and rehashing
	- Return Value
		- If success, 0 will be returned. If failed, 1 will be returned.

