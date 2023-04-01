#include <stdint.h>
#include <stddef.h>

typedef struct HNode {
	HNode *next = NULL;
	uint64_t hcode = 0;
} HNode;

typedef struct HTab {
	HNode **tab = NULL;
	size_t mask = 0;
	size_t size = 0;
	
} HTab;

// real hashtable interface
// uses 2 hashtables for progressive resizing
typedef struct HMap {
	HTab ht1;
	HTab ht2;
	size_t resizing_pos = 0;
}HMap;

HNode *hm_lookup(HMap *hmap, HNode *key, bool (*cmp)(HNode *, HNode *));
void hm_insert(HMap *hmap, HNode *node);
HNode *hm_pop(HMap *hmap, HNode *key, bool (*cmp)(HNode *, HNode *));
void hm_destroy(HMap *hmap);
