#include <datastructures/linkedlist.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdlib.h>

void ll_create_test(void)
{
	linkedlist_t* ll = ll_create();
	CU_ASSERT_PTR_NOT_NULL_FATAL(ll);
	CU_ASSERT_PTR_NULL(ll->head);
	CU_ASSERT_EQUAL(ll->size, 0);
	ll_destroy(ll, NULL);
}

void ll_add_test(void)
{
	linkedlist_t* ll = ll_create();
	
	for(int i = 0; i < 100; i++)
	{
		int* x = malloc(sizeof(int));
		*x = i;		
		ll_add(ll, x);
		CU_ASSERT_EQUAL(ll->size, i + 1);
	}

	ll_node_t* node = ll->head;
	for(int i = 99; i >= 0; i--)
	{
		CU_ASSERT_PTR_NOT_NULL_FATAL(node);		
		CU_ASSERT_EQUAL(*((int*)(node->object)), i);	
		node = node->next;
	}

	ll_destroy(ll, free);
}