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
	
	int* x = (int*)malloc(sizeof(int));
	*x = 12;
	int* y = (int*)malloc(sizeof(int));
	*y = 13;
	int* z = (int*)malloc(sizeof(int));
	*z = 14;
	int* w = (int*)malloc(sizeof(int));
	*w = 15;

	ht_put(ht, (char*)"Abdul", x);
	ht_put(ht, (char*)"Em", y);
	ht_put(ht, (char*)"Moh", z);
	ht_put(ht, (char*)"Sal", w);


	CU_ASSERT_PTR_EQUAL(ht_get(ht,(char*) "Abdul"), x);
	CU_ASSERT_PTR_EQUAL(ht_get(ht, (char*)"Em"), y);
	CU_ASSERT_PTR_EQUAL(ht_get(ht, (char*)"Moh"), z);
	CU_ASSERT_PTR_EQUAL(ht_get(ht, (char*)"Sal"), w);

	CU_ASSERT(ht_contains_key(ht, (char*)"Abdul"));
	CU_ASSERT(ht_contains_key(ht, (char*)"Em"));
	CU_ASSERT(ht_contains_key(ht, (char*)"Moh"));
	CU_ASSERT(ht_contains_key(ht, (char*)"Sal"));


	ht_put(ht, (char*)"Abdul", y);
	CU_ASSERT_PTR_EQUAL(ht_get(ht, (char*)"Abdul"), y);
	free(x);

	free(ht_remove(ht, (char*) ht_remove(ht,(char*) "Abdul")));
	CU_ASSERT(!ht_contains_key(ht, (char*)"Abdul"));
	
	ht_destroy(ht, free);
}