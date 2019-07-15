/**
  * MIT License
  *
  * Copyright (c) 2019 Abdullah Emad
  * 
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to deal
  * in the Software without restriction, including without limitation the rights
  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  * copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  * 
  * The above copyright notice and this permission notice shall be included in all
  * copies or substantial portions of the Software.
  * 
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */

#include <ndn-cxx/face.hpp>
#include <modules/FaceManager.hpp>
#include <vector>
#include <ndnpi.hpp>
#include <data/Interface.hpp>
#include <unordered_set>
#include <string>
#include <assert.h>
#include <iostream>
using namespace ndn;
using namespace std;

FaceManager::~FaceManager(void)
{

}

void FaceManager::addRequest(Interest interest)
{
	vector<Interface*> interfaces =  fib->computeMatchingFaces(interest);
    assert(interfaces.size() <= 1);
	for(Interface* interface : interfaces)
	{
		string namestr = interest.getName().toUri();
        interface->expressInterest(interest,  
                           bind(&FaceManager::onData, this,  _1, _2),
                           bind(&FaceManager::onNack, this, _1, _2),
                           bind(&FaceManager::onTimeout, this, _1));
        std::cout << namestr << " Forwarded to: " << interface->getIp() << "\n";
	}
}




void FaceManager::sendAll(void)
{
	processEventsForAllInterfaces();
}



void FaceManager::onData(const Interest& interest, const Data& data)
{
    /*send it back*/   
    stream->putData(data);
}

void FaceManager::onTimeout(const Interest& interest)
{

}
void FaceManager::onNack(const Interest& interest, const lp::Nack& nack)
{
    stream->putNack(nack);
}

/***********************
 *   private helpers   *
 ***********************/

void FaceManager::processEventsForAllInterfaces(void)
{
    vector<Interface*> interfaces = fib->getInterfaces();
	for(Interface* interface : interfaces)
	{
		/*non-blocking function that will start sending all the interests*/
		interface->processEvents();
	}
}



