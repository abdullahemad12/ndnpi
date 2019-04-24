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

#include <modules/RequestsThread.hpp>
#include <data/Request.hpp>
#include <data/Interface.hpp>
#include <thread>

RequestsThread::RequestsThread(Interest* interest, vector<Interface*> faces, PendingInterestTable* pit, ForwardingInformationBase* fib)
{
	for(int i = 0, n = faces.size(); i < n; i++)
	{
		Request* request = new Request(faces[i], interest, this, pit, fib);
		this->requests.push_back(request);
	}
	this->pit = pit;
	this->fib = fib;
	this->n_requests = faces.size();
	this->interest = interest;
}

void RequestsThread::t_func(void)
{
	for(int i = 0, n = this->requests.size(); i < n; i++){
		this->requests[i]->expressInterest();
	}
}
void RequestsThread::run(void)
{

	//this->t = new thread(bind(&RequestsThread::t_func, this));
	this->t_func();
}
void RequestsThread::decrementRequests(void)
{
	--this->n_requests;
	if(this->n_requests <= 0)
	{
		this->pit->getMatchingEntry((Name*)&this->interest->getName());
		for(int i = 0, n = this->requests.size(); i < n; i++)
		{
			delete requests[i];
		}
		delete this;	
	}
}