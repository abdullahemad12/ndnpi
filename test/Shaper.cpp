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



class ShaperFaceManagerTester1 : public FaceManager
{
	public:
	vector<Interest> expected;
	vector<Interest> interests;

	~ShaperFaceManagerTester1(void)
	{

	}
	
	ShaperFaceManagerTester1(vector<Interest> expected)
	{
		this->expected = expected;
	}

	void addRequest(Interest interest)
	{
		interests.push_back(interest);
	}
	void sendAll(void)
	{
		CU_ASSERT_EQUAL(interests.size(), expected.size());
		CU_ASSERT(interests == expected);
	}
};

/**
  * This test will add 100 packets of to a shaper of capacity 90 and priority 2 and see how many packets 
  * is the shaper going to forward to the FaceManager
  */
void shaper_forward_test1(void)
{
	shaper = new Shaper(90);
	shaper->setWeight(0.50, 0);
	shaper->setWeight(0.25, 1);
	shaper->setWeight(0.15, 2);
	shaper->setWeight(0.10, 3);

	vector<Interest> interests;
	vector<Interest> expected; // expected is the first 90 packets since the capacity is 90 

	for(int i = 0; i < 100; i++)
	{
		Interest interest(Name(names[i % 17]));
		interest.setPriority(2);
		interests.push_back(interest);
	}

	for(int i = 0; i < 90; i++)
	{
		expected.push_back(interests[i]);
	}
	
	ShaperFaceManagerTester1 manager(expected);
	faceManager = &manager;
	for(Interest interest : interests)
	{
		shaper->addInterest(interest);
	}

	shaper->run();



	delete shaper;
}

/**
  * Create a new shaper with capacity of 300, then add 100 Interest Packet of each priority
  * the expected out put should contain:
  * - 100 packet of priority 0
  * - 75 packets of priority 1
  * - 45 packets of priority 2
  * - 30 packets of priority 3
  */
void shaper_forward_test2(void)
{
	shaper = new Shaper(300);
	shaper->setWeight(0.50, 0);
	shaper->setWeight(0.25, 1);
	shaper->setWeight(0.15, 2);
	shaper->setWeight(0.10, 3);
	
	vector<Interest> interests0;
	vector<Interest> interests1;
	vector<Interest> interests2;
	vector<Interest> interests3;

	vector<Interest> expected; // expected is the first 90 packets since the capacity is 90 

	for(int i = 0; i < 100; i++)
	{
		Interest interest(Name(names[i % 17]));
		interest.setPriority(0);
		interests0.push_back(interest);
	}

	for(int i = 0; i < 100; i++)
	{
		Interest interest(Name(names[i % 17]));
		interest.setPriority(1);
		interests1.push_back(interest);
	}

	for(int i = 0; i < 100; i++)
	{
		Interest interest(Name(names[i % 17]));
		interest.setPriority(2);
		interests2.push_back(interest);
	}
	
	for(int i = 0; i < 100; i++)
	{
		Interest interest(Name(names[i % 17]));
		interest.setPriority(3);
		interests3.push_back(interest);
	}

	for(int i = 0; i < 100; i++)
	{
		expected.push_back(interests0[i]);
	}

	for(int i = 0; i < 75; i++)
	{
		expected.push_back(interests1[i]);
	}

	for(int i = 0; i < 45; i++)
	{
		expected.push_back(interests2[i]);
	}
	for(int i = 0; i < 30; i++)
	{
		expected.push_back(interests3[i]);
	}
	

	
	ShaperFaceManagerTester1 manager(expected);
	faceManager = &manager;
	for(int i = 0; i < 100; i++)
	{
		shaper->addInterest(interests0[i]);
		shaper->addInterest(interests1[i]);
		shaper->addInterest(interests2[i]);
		shaper->addInterest(interests3[i]);
	}
	

	shaper->run();



	delete shaper;
}

/**
  * With a capacity of 300 interests, put 150 interests of priority 1 and 150 of priority 2. Make sure that
  * the percentage of priority 0 and priority 3 is divided among priority 1 and 2 that is
  * - 150 packets of priority 1 will be sent to the FaceManager
  * - 135 packets of priority 2 will be sent to the FaceManager
  */
void shaper_forward_test3(void)
{
	shaper = new Shaper(300);
	shaper->setWeight(0.50, 0);
	shaper->setWeight(0.25, 1);
	shaper->setWeight(0.15, 2);
	shaper->setWeight(0.10, 3);


	vector<Interest> interests1;
	vector<Interest> interests2;
	vector<Interest> expected;	

	for(int i = 0; i < 150; i++)
	{
		Interest interest(Name(names[i % 17]));
		interest.setPriority(1);
		interests1.push_back(interest);
		expected.push_back(interest);
	}

	for(int i = 0; i < 150; i++)
	{
		Interest interest(Name(names[i % 17]));
		interest.setPriority(2);
		interests2.push_back(interest);
	}

	for(int i = 0; i < 135; i++)
	{
		expected.push_back(interests2[i]);
	}


	
	ShaperFaceManagerTester1 manager(expected);
	faceManager = &manager;
	for(int i = 0; i < 150; i++)
	{
		shaper->addInterest(interests1[i]);
		shaper->addInterest(interests2[i]);
	}
	delete shaper;
}
