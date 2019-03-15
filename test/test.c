#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "linkedlist.c"
#include "hashtable.c"

/*prototypes*/
static int init(void);
static int run_tests(void);
static int init_suite(void);
static int clean_suite(void);

/*Array of all the tests*/
struct test
{
	const char *name;
	void (*func) (void);
};

static struct test utests[] = {
	{"ll_create_test", ll_create_test},
	{"ll_add_test", ll_add_test},
	{"ll_get_at_test", ll_get_at_test},
	{"ll_remove_at_test", ll_remove_at_test},
	{"ht_create_test", ht_create_test},
	{"ht_test", ht_test},
	{NULL, NULL}

};

static struct test ctests[] = {
	{NULL, NULL}

};

int main(void)
{

	int err;

	/*initialize */
	err = init();
	if (err != CUE_SUCCESS)
	{
		return err;
	}
	/* Run all tests */
	return run_tests();

}

static int createSuite(const char *suite_name, struct test tests[])
{
	CU_pSuite pSuite = NULL;

	/* add a suite to the registry */
	pSuite = CU_add_suite(suite_name, init_suite, clean_suite);
	if (NULL == pSuite)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	for (int i = 0; tests[i].name != NULL && tests[i].func != NULL; i++)
	{
		if ((NULL == CU_add_test(pSuite, tests[i].name, tests[i].func)))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	}
	return CUE_SUCCESS;
}

static int init(void)
{
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	int err = createSuite("Suite 1", utests);

	if (err != CUE_SUCCESS)
	{
		return err;
	}

	err = createSuite("Suite 2", ctests);
	return err;

}

static int run_tests(void)
{
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}

static int init_suite(void)
{
	return 0;
}

static int clean_suite(void)
{
	return 0;
}
