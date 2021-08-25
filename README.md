# Simple Hash Map for ip/dn mapping

## Entry data structure
```sh=
#define DNS_MAX_LEN 255
struct entry {
	uint32_t ip;
	char dn[DNS_MAX_LEN + 1];
};
```

## Functions
- int map_init(struct map *map);
	- Initialize the map data structure
	- Return Value
		- If success, 0 will be returned. If failed, 1 will be returned.
- uint32_t hash1(uint32_t key, unsigned int cap); 
	- Calculate the hash value according to the given key which is ip in these design
	- Return value
		- The hash value corresponding to the key.
- uint32_t find(struct map *map, uint32_t key);
	- Find the empty slot or the slot of the key
	- Return Value
		- The hash value of the slot.
- uint32_t insert(struct map *map, struct entry *entry);
	- Insert the entry into the map
	- Return value
		- The hash value where this entry is inserted to.
- int expand(struct map *map);
	- expand the bucket size of the map and rehashing
	- Return Value
		- If success, 0 will be returned. If failed, 1 will be returned.
- void printmap(struct map *map);
	- Print out all the entries in the map

