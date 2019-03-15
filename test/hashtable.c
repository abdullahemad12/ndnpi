#include <datastructures/hashtable.h>

void ht_create_test(void)
{
	hashtable_t* ht = ht_create(100);
	CU_ASSERT_PTR_NOT_NULL_FATAL(ht);
	CU_ASSERT_EQUAL(ht->size, 0);
	CU_ASSERT_EQUAL(ht->n, 100);
	ht_destroy(ht, NULL);
}

void ht_test(void)
{
	hashtable_t* ht = ht_create(100);
	
	int* x = malloc(sizeof(int));
	*x = 12;
	int* y = malloc(sizeof(int));
	*y = 13;
	int* z = malloc(sizeof(int));
	*z = 14;
	int* w = malloc(sizeof(int));
	*w = 15;

	ht_put(ht, "Abdul", x);
	ht_put(ht, "Em", y);
	ht_put(ht, "Moh", z);
	ht_put(ht, "Sal", w);


	CU_ASSERT_PTR_EQUAL(ht_get(ht, "Abdul"), x);
	CU_ASSERT_PTR_EQUAL(ht_get(ht, "Em"), y);
	CU_ASSERT_PTR_EQUAL(ht_get(ht, "Moh"), z);
	CU_ASSERT_PTR_EQUAL(ht_get(ht, "Sal"), w);

	CU_ASSERT(ht_contains_key(ht, "Abdul"));
	CU_ASSERT(ht_contains_key(ht, "Em"));
	CU_ASSERT(ht_contains_key(ht, "Moh"));
	CU_ASSERT(ht_contains_key(ht, "Sal"));


	ht_put(ht, "Abdul", y);
	CU_ASSERT_PTR_EQUAL(ht_get(ht, "Abdul"), y);
	free(x);

	free(ht_remove(ht, ht_remove(ht, "Abdul")));
	CU_ASSERT(!ht_contains_key(ht, "Abdul"));
	
	ht_destroy(ht, free);
}