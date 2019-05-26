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
#include <data/Request.hpp>
#include <vector>
#include <ndnpi.hpp>
#include <data/Interface.hpp>
#include <unordered_set>
#include <string>

using namespace ndn;
using namespace std;

void FaceManager::addRequest(Interest interest)
{
	vector<Interface*> interfaces =  fib->computeMatchingFaces(interest.getName());
	for(Interface* interface : interfaces)
	{
		string namestr = interest.getName().toUri();
		currentNames.insert(namestr);
		Request request(interest, interface);
		request.addObserver(this);		
		requests.push(request);
		this->interfaces.insert(interface);
	}
}




void FaceManager::sendAll(void)
{
	
	expressAllInterests();

	processEventsForAllInterfaces();
	
	/*will wait until response for all interests has been recieved*/
	joinAllInterfaces();

	interfaces.clear();

	/*send nacks as necessary*/
	sendNacks();
}

// timeout
void FaceManager::update(RequestSubject* subject)
{
	// do nothing for now 
}

// data
void FaceManager::update(RequestSubject* subject, const Data& data)
{

	stream->putData(data); /*put data*/
	
	Request* request = (Request*) subject;
	currentNamesLock.lock();	
	fib->insert(*request);
	/*TODO: add function that gets name uri to requests */
	string namestr = request->getInterest().getName().toUri();
	currentNames.erase(namestr);
	currentNamesLock.unlock();	
}

// Nack
void FaceManager::update(RequestSubject* subject, const lp::Nack& nack)
{
	nackslock.lock();
	/*must delete it*/
	const lp::Nack* nacknew = new lp::Nack(nack);
	nacks.push(nacknew);

	nackslock.unlock();
}


/***********************
 *   private helpers   *
 ***********************/

void FaceManager::expressAllInterests(void)
{
	while(!requests.empty())
	{
		Request request = requests.front();
		requests.pop();
		request.expressInterest();
	}
}

void FaceManager::processEventsForAllInterfaces(void)
{
	for(Interface* interface : interfaces)
	{
		/*non-blocking function that will start sending all the interests*/
		interface->processEvents();
	}
}


void FaceManager::joinAllInterfaces(void)
{
	/*Makes sure all the interest was send and all the responses was recieved*/
	for(Interface* interface : interfaces)
	{
		interface->join();
	}
}


void FaceManager::sendNacks(void)
{
	while(!nacks.empty())
	{
		const lp::Nack* nack = nacks.front();
		nacks.pop();
		string namestr = nack->getInterest().getName().toUri();	
		if(currentNames.find(namestr) != currentNames.end())
		{
			stream->putNack(*nack);
			currentNames.erase(namestr);
		}
		delete nack;
	}
	currentNames.clear();
}


