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


#ifndef _MODULES_REQUESTSTHREAD_
#define _MODULES_REQUESTSTHREAD_

#include <ndn-cxx/face.hpp>
#include <thread>
#include <vector>
#include <modules/ForwardingInformationBase.hpp>
#include <modules/PendingInterestTable.hpp>
#include <data/FIBEntry.hpp>
#include <mutex>
#include <data/Interface.hpp>
using namespace std;

class Request;

class RequestsThread
{
	private:
		mutex lock;
		thread* t;
		int n_requests;
		vector<Request*> requests;
		ForwardingInformationBase* fib;
		Interest* interest;
		void t_func(void);
	public:
		RequestsThread(Interest* interest, vector<Interface*> faces, ForwardingInformationBase* fib);
		void run(void);
		int decrementRequests(void);
};


#endif /*..._MODULES_REQUESTSTHREAD_*/
