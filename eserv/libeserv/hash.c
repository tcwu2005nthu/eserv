#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "hash.h"

static int ex_hashcmp_str(const char *s1, const char *s2)
{
	return (0 == strcmp(s1, s2));
}

void ex_hash_init(ex_hashmap *hm, ex_mpool *mp, size_t _size)
{
	assert(mp != NULL);
	hm->mpool = mp;
	hm->buckets =
		(ex_hashlist **) ex_mpool_malloc(mp,
		                                 sizeof(int) * _size);
	memset(hm->buckets, 0, sizeof(int) * _size);
	hm->size = _size;

	hm->hashcmp = (void *) ex_hashcmp_str;
	hm->hashfun = (void *) ex_hashfun_str;
}

void ex_hash_clear(ex_hashmap *hm)
{
	ex_hashlist *nl, *cl;
	size_t i;

	do {
		if (hm->mpool->cflag == 0)
			break;
		for (i = 0 ; i < hm->size ; ++i) {
			cl = hm->buckets[i];
			while (cl != NULL) {
				nl = cl->next;
				ex_mpool_free(hm->mpool, cl);
				cl = nl;
			}
		}
		ex_mpool_free(hm->mpool, hm->buckets);
	} while (0);

	memset(hm, 0, sizeof(*hm));
}

int ex_hash_add(ex_hashmap *hm, const void *key, const void *value)
{
	int pos = hm->hashfun(key) % hm->size;
	ex_hashlist *nhl =
		(ex_hashlist *) ex_mpool_malloc(hm->mpool,
		                                sizeof(ex_hashlist));

	nhl->key = (void *) key, nhl->value = (void *) value;
	nhl->next = hm->buckets[pos];
	hm->buckets[pos] = nhl;

	return pos;
}

void* ex_hash_find(const ex_hashmap *hm, const void *key)
{
	int pos = hm->hashfun(key) % hm->size;
	ex_hashlist *nlh = hm->buckets[pos];
	void *ret = NULL;
	while (nlh != NULL) {
		if (hm->hashcmp(nlh->key, key)) {
			ret = nlh->value;
			break;
		}
		nlh = nlh->next;
	}
	return ret;
}

int ex_hashfun_str(const char *s)
{
	int even , odd;
	int i = 0;
	int mask = 0x1F;
	int ret;
	even = odd = 0;
	while (*s != '\0') {
		if (i & 1)
			odd ^= *s;
		else
			even ^= *s;
		++s;
		++i;
	}
	ret = even & mask;
	ret <<= 5;
	ret += (odd & mask);
	return ret;
}

int ex_hashfun_prime(const char *s)
{
	/*2 ,3 ,5 ,7 ,11 ,13 ,17,19,23,29,31*/
	int primeflag[32] = {
		0, 0, 1, 1, 0, 1, 0, 1,
		0, 0, 0, 1, 0, 1, 0, 0,
		0, 1, 0, 1, 0, 0, 0, 1,
		0, 0, 0, 0, 0, 1, 0, 1,
	};
	int mask = 0x1F;
	int i = 0;
	int pValue = 0;
	int nValue = 0;
	int ret;
	while (*s) {
		if (i < 32 && primeflag[i]) pValue ^= *s;
		else nValue ^= *s;
		++s;
		++i;
	}
	ret = pValue & mask;
	ret <<= 5;
	ret += nValue & mask;

	return ret;
}

int ex_hashfun_elf(const char *s)
{
	unsigned long h, g;
	h = 0;
	while (*s) {
		h = (h << 4) + *s++;
		g = h & 0xf0000000;
		if (g)
			h ^= g >> 24;
		h &=  ~g;
	}
	return h;
}

