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


#include <string>
#include <data/Interface.hpp>
#include <ndn-cxx/face.hpp>
#include <thread>
#include <iostream>
#include <ndnpi.hpp>

using namespace std;
using namespace ndn;
Interface::Interface(int id, string ip, string port)
{
    this->id = id;
	this->ip = ip;
	this->port = port;
	face = NULL;
    t = NULL;
}

void Interface::connectToFace(void)
{
    face = new Face(ip, port);
}

Interface::~Interface(void)
{
    if(face != NULL)
    {
    	face->shutdown();
    	delete face;
    }
}

string Interface::getIp(void)
{
	return this->ip;
}

string Interface::getPort(void)
{
	return this->port;
}


int Interface::getId(void)
{
    return this->id;
}

void Interface::expressInterest(const Interest &interest, const DataCallback &afterSatisfied, 
							 const NackCallback &afterNacked, const TimeoutCallback &afterTimeout)
{
	afterNackeds.push_back(afterNacked);
	interests.push_back(interest);
	face->expressInterest(interest, afterSatisfied, afterNacked, afterTimeout);
}

void Interface::t_func(void)
{
    while(1)
    {
	    try
	    {
            sem->wait();
		    // try processing the event
		    face->processEvents();
	    }
	    catch (const std::exception& e)
	    {
		    cout << "Couldn't forward on this face\n";
		    /*
		     * this face is down
             * Important to send back an Nack to prevent 
		     * the client from waiting on data that is never coming back
		     * and also to update the FIB to execlude this interface later
             */
		    for(unsigned int i = 0; i < interests.size(); i++)
		    {
			    lp::Nack nack(interests[i]);
			    nack.setReason(lp::NackReason::NONE);
			    afterNackeds[i](interests[i], nack);
		    }
		    
		    /*clear the lists*/
		    afterNackeds.clear();
		    interests.clear();
	    }
    }
}

void Interface::processEvents(void)
{
	/*create a new thread so it does not block
      the execution of the parent thread while sending the
      packets*/
    if(t == NULL)
    {
        sem = new Semaphore();
        t = new thread(bind(&Interface::t_func, this));
    }
    
    sem->notify();

}
