#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <modules/ForwardingInformationBase.hpp>
#include <datastructures/linkedlist.h>
#include <ndn-cxx/face.hpp>

const char* names[] = {
	"/test/app/ndn",
	"/test/daron/ndn/ip",
	"/test/schubert/api",
	"/test/app/api/ip",
	"/test/app/api/name",
	"/test/app/green/day",
	"/test/app/green/eye",
	"/cranberries/app/glass/floor",
	"/trees/xerces/space",
	"/wallet/clothes/gemuse",
	"/chili/peppers/app",
	"/chicken/voices/will",
	"/electric/waves/on/off",
	"/sail/off/run/today",
	"/test/down/victims",
	"/lost/wild/ndn/api",
	"/dreams/clothes/ip",
	"/injustice/space/gloor"
};



void fib_constructor_test(void)
{
	chdir("test");
	/**
	  * Because this is pretty hard to test because of the lack of public function
      * It will be inspected by gdb
      */
	ForwardingInformationBase* fib = new ForwardingInformationBase("routingtable/rt1");
	delete fib;
}


void fib_lpm_test(void)
{
	chdir("test");
	
	ForwardingInformationBase* fib = new ForwardingInformationBase("routingtable/rt1");


	struct linkedlist* faces = fib->getFaces();


	for(int i = 0; i < 3; i++)
	{
		Face* face = (Face*) ll_get_at(faces, i);
		for(int j = 0; j < 6; j++)
		{
			Name* name = new Name(names[i * 6 + j]);
			fib->insert(name, face);
		}
	}

	
	Name* name = new Name("/test/down/system/p");
	vector<Face*> interfaces = fib->computeMatchingFaces(name);
	CU_ASSERT_PTR_EQUAL(interfaces[0], ll_get_at(faces, 2));

	delete name;

	name = new Name("/trees/resistance/eighty");
	interfaces = fib->computeMatchingFaces(name);
	CU_ASSERT_PTR_EQUAL(interfaces[0], ll_get_at(faces, 1));
	delete name;

	name = new Name("/test/app/api/boy");
	interfaces = fib->computeMatchingFaces(name);
	CU_ASSERT_PTR_EQUAL(interfaces[0], ll_get_at(faces, 0));
	delete name;
	
	/*should be a broadcast*/
	name = new Name("/non/existing/name");
	interfaces = fib->computeMatchingFaces(name);
	CU_ASSERT_PTR_EQUAL(interfaces[0], ll_get_at(faces, 0));
	CU_ASSERT_PTR_EQUAL(interfaces[1], ll_get_at(faces, 1));
	CU_ASSERT_PTR_EQUAL(interfaces[2], ll_get_at(faces, 2));
	delete name;

	
	delete fib;
}
