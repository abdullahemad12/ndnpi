#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <modules/ForwardingInformationBase.hpp>
#include <modules/Shaper.hpp>
#include <modules/FaceManager.hpp>
#include <modules/Stream.hpp>
#include <data/Interface.hpp>
#include <datastructures/linkedlist.h>
#include <ndn-cxx/face.hpp>
#include <fstream>
#include <iostream>
#include <ndnpi.hpp>

using namespace std;

ForwardingInformationBase* fib = NULL;
Shaper* shaper = NULL;
FaceManager* faceManager = NULL;
Stream* stream = NULL;

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
	fib = new ForwardingInformationBase("routingtable/rt1");

	ifstream myfile;
	myfile.open ("routingtable/rt1");


	string id;
	string ip;
	string port;
	int size = 0;
	while(myfile >> id >> ip >> port)
	{
		++size;
		vector<Interface*> interfaces = fib->getInterfaces();
		bool found = false;
		for(Interface* interface : interfaces)
		{
			if(interface->getIp().compare(ip) == 0 && interface->getPort().compare(port) == 0)
			{
				CU_ASSERT(!found);
				found = true;
			}
		}
		CU_ASSERT(found);
	}
	CU_ASSERT_EQUAL(size, fib->getInterfaces().size());

	delete fib;
}


void fib_lpm_test(void)
{
	chdir("test");
	
	ForwardingInformationBase* fib = new ForwardingInformationBase("routingtable/rt1");
	
			


	delete fib;
}
