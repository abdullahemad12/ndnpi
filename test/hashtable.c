#include <datastructures/hashtable.h>

void ht_create_test(void)
{
	hashtable_t* ht = ht_create(100);
	CU_ASSERT_PTR_NOT_NULL_FATAL(ht);
	CU_ASSERT_EQUAL(ht->size, 0);
	CU_ASSERT_EQUAL(ht->n, 100);
	ht_destroy(ht, NULL);
}