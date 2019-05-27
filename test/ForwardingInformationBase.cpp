#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <modules/ForwardingInformationBase.hpp>
#include <modules/Shaper.hpp>
#include <modules/FaceManager.hpp>
#include <modules/Stream.hpp>
#include <data/Interface.hpp>
#include <data/Request.hpp>
#include <datastructures/linkedlist.h>
#include <ndn-cxx/face.hpp>
#include <fstream>
#include <iostream>
#include <ndnpi.hpp>
#include <string>
#include <unordered_map>

using namespace std;

ForwardingInformationBase* fib = NULL;
Shaper* shaper = NULL;
FaceManager* faceManager = NULL;
Stream* stream = NULL;

string names[] = {
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
	"/injustice/space/gloor",
	""
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
	fib = NULL;
}

/**
  * Test the case where the ForwardingInformationBase is initially empty
  */
void fib_lpm_test1(void)
{
	chdir("test");
	
	fib = new ForwardingInformationBase("routingtable/rt1");
	
	Interest interest1(Name("/test/app/green/eye"));
	Interest interest2(Name("/chicken/voices/will"));
	Interest interest3(Name("/electric/waves/on/off"));
	Interest interest4(Name("/injustice/space/gloor"));


	vector<Interface*> interfaces;

	interfaces = fib->computeMatchingFaces(interest1);
	CU_ASSERT(interfaces == fib->getInterfaces());
	

	interfaces = fib->computeMatchingFaces(interest2);
	CU_ASSERT(interfaces == fib->getInterfaces());

	interfaces = fib->computeMatchingFaces(interest3);
	CU_ASSERT(interfaces == fib->getInterfaces());


	interfaces = fib->computeMatchingFaces(interest4);
	CU_ASSERT(interfaces == fib->getInterfaces());
	delete fib;
}

/**  
  * Insert some entries in the table and try to compute a name that does not exist 
  * in the table. It should return all the interfaces
  */
void fib_lpm_test2(void)
{
	chdir("test");
	
	fib = new ForwardingInformationBase("routingtable/rt1");
	vector<Interface*> interfaces = fib->getInterfaces();

	for(int i = 0; names[i].length() != 0; i++)
	{
		Interest interest(Name(names[i]));
		Request request(interest, interfaces[i % interfaces.size()]);
		
		fib->insert(request);
	}
	

	Interest interest1(Name("/melody/mira/cat"));
	CU_ASSERT(interfaces == fib->computeMatchingFaces(interest1));

	Interest interest2(Name("/uni/stutt/gart"));
	CU_ASSERT(interfaces == fib->computeMatchingFaces(interest2));
	delete fib;
}

/**
  * Insert Some Entries and check that it returns the most interface that this entry lpm matches
  * the priority of these interest is 3
  */
void fib_lpm_test3(void)
{
	fib = new ForwardingInformationBase("routingtable/rt1");
	vector<Interface*> interfaces = fib->getInterfaces();
	unordered_map<string, Interface*> map;

	for(int i = 0; names[i].length() != 0; i++)
	{
		Interest interest(Name(names[i]));
		Request request(interest, interfaces[i % interfaces.size()]);
		map[names[i]] = interfaces[i % interfaces.size()];
		fib->insert(request);
	}

	
	/*now try some matching some names that is similar*/

	Interest interest1(Name("/cranberries/melody/app"));
	interest1.setPriority(3);
	interfaces = fib->computeMatchingFaces(interest1);
	CU_ASSERT_EQUAL(interfaces.size(), 1);
	CU_ASSERT_EQUAL(interfaces[0], map[names[7]]);		
	
	Interest interest2(Name("/test/down/system"));
	interest2.setPriority(3);
	interfaces = fib->computeMatchingFaces(interest2);
	CU_ASSERT_EQUAL(interfaces.size(), 1);
	CU_ASSERT_EQUAL(interfaces[0], map[names[14]]);		
	

	Interest interest3(Name("/trees/zizo"));
	interest3.setPriority(3);
	interfaces = fib->computeMatchingFaces(interest3);
	CU_ASSERT_EQUAL(interfaces.size(), 1);
	CU_ASSERT_EQUAL(interfaces[0], map[names[8]]);		
	

	Interest interest4(Name("/chicken/and/meat"));
	interest4.setPriority(3);
	interfaces = fib->computeMatchingFaces(interest4);
	CU_ASSERT_EQUAL(interfaces.size(), 1);
	CU_ASSERT_EQUAL(interfaces[0], map[names[11]]);		
	

	
	delete fib;
}

/**
  * same as test 3 but with mixed priorities
  */
void fib_lpm_test4(void)
{
	fib = new ForwardingInformationBase("routingtable/rt1");
	vector<Interface*> interfaces = fib->getInterfaces();
	unordered_map<string, Interface*> map;

	for(int i = 0; names[i].length() != 0; i++)
	{
		Interest interest(Name(names[i]));
		Request request(interest, interfaces[i % interfaces.size()]);
		map[names[i]] = interfaces[i % interfaces.size()];
		fib->insert(request);
	}

	
	/*now try some matching some names that is similar*/

	Interest interest1(Name("/cranberries/melody/app"));
	interest1.setPriority(1);
	interfaces = fib->computeMatchingFaces(interest1);
	CU_ASSERT_EQUAL(interfaces.size(), 2);
	CU_ASSERT_EQUAL(interfaces[0], map[names[7]]);	
	CU_ASSERT_EQUAL(interfaces[1], fib->getInterfaces()[2]);			

	Interest interest2(Name("/test/down/system"));
	interest2.setPriority(3);
	interfaces = fib->computeMatchingFaces(interest2);
	CU_ASSERT_EQUAL(interfaces.size(), 1);
	CU_ASSERT_EQUAL(interfaces[0], map[names[14]]);
	

	Interest interest3(Name("/trees/zizo"));
	interest3.setPriority(2);
	interfaces = fib->computeMatchingFaces(interest3);
	CU_ASSERT_EQUAL(interfaces.size(), 1);
	CU_ASSERT_EQUAL(interfaces[0], map[names[8]]);
	

	Interest interest4(Name("/chicken/and/meat"));
	interest4.setPriority(0);
	interfaces = fib->computeMatchingFaces(interest4);
	CU_ASSERT_EQUAL(interfaces.size(), fib->getInterfaces().size());

	

	
	delete fib;
}
