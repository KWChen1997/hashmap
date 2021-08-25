# Simple Hash Map for ip/dn mapping

## Entry data structure
```sh=
#define DNS_MAX_LEN 255
struct entry {
	uint32_t key;
	void *nxt;
	...
};
```
- please make sure memcpy will deliver a deep copy of your entry

## Functions
- int map_init(struct map *map, size_t entry_size, uint16_t increm);
	- Initialize the map data structure
	- Return Value
		- If success, 0 will be returned. If failed, 1 will be returned.
- int hash(uint32_t key, unsigned int cap); 
	- Calculate the hash value according to the given key which is ip in these design
	- Return value
		- The hash value corresponding to the key.
- void **find(struct map *map, uint32_t key);
	- Find the empty slot or the slot of the key
	- Return Value
		- If there is a hit, the return pointer will point to the address of the entry. If it is a miss, the return pointer will point to a NULL address
- uint32_t insert(struct map *map, struct node *entry);
	- Insert the entry into the map
	- Return value
		- If success, 0 will be returned. If failed, 1 will be returned.
- int expand(struct map *map);
	- expand the bucket size of the map and rehashing
	- Return Value
		- If success, 0 will be returned. If failed, 1 will be returned.

