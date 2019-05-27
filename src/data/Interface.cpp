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


using namespace std;
using namespace ndn;
Interface::Interface(string ip, string port)
{
	this->ip = ip;
	this->port = port;
	face = new Face(ip, port);
}

Interface::~Interface(void)
{
	face->shutdown();
	delete face;
}

string Interface::getIp(void)
{
	return this->ip;
}

string Interface::getPort(void)
{
	return this->port;
}

void Interface::expressInterest(const Interest &interest, const DataCallback &afterSatisfied, 
							 const NackCallback &afterNacked, const TimeoutCallback &afterTimeout)
{
	face->expressInterest(interest, afterSatisfied, afterNacked, afterTimeout);
}

void Interface::t_func(void)
{
	face->processEvents();
}

void Interface::processEvents(void)
{
	/*create a new thread so it does not block
      the execution of the parent thread while sending the
      packets*/
	t = new thread(bind(&Interface::t_func, this));
}

void Interface::join(void)
{
	t->join();
	delete t;
}
