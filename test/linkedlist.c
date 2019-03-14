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

void ll_get_at_test(void)
{

	linkedlist_t* ll = ll_create();
	
	for(int i = 0; i < 10; i++)
	{
		int* x = malloc(sizeof(int));
		*x = i;		
		ll_add(ll, x);
	}
		
	int j = 9;
	for(unsigned int i = 0; i < 10; i++)
	{
		int* x = (int*)ll_get_at(ll, i);
		CU_ASSERT_PTR_NOT_NULL_FATAL(x);
		CU_ASSERT_EQUAL(*x, j--);
	}

	ll_destroy(ll, free);

}

void ll_remove_at_test(void)
{
	linkedlist_t* ll = ll_create();
	
	for(int i = 0; i < 10; i++)
	{
		int* x = malloc(sizeof(int));
		*x = i;		
		ll_add(ll, x);
	}
	
	int* x = ll_remove_at(ll, 5);

	CU_ASSERT_PTR_NOT_NULL(x);
	CU_ASSERT_EQUAL(*x, 4);	
	CU_ASSERT_EQUAL(ll->size, 9);

	ll_node_t* llnode = ll->head; 
	while(llnode != NULL)
	{
		int* y = llnode->object;
		CU_ASSERT_NOT_EQUAL(*y, 4);
		llnode = llnode->next;
	}

	x  = ll_remove_at(ll, 0);
	llnode = ll->head; 
	while(llnode != NULL)
	{
		int* y = llnode->object;
		CU_ASSERT_NOT_EQUAL(*y, 9);
		llnode = llnode->next;
	}




	x  = ll_remove_at(ll, 7);
	llnode = ll->head; 
	while(llnode != NULL)
	{
		int* y = llnode->object;
		CU_ASSERT_NOT_EQUAL(*y, 0);
		llnode = llnode->next;
	}
	CU_ASSERT_EQUAL(ll->size, 7);

	
	ll_destroy(ll, free);
}