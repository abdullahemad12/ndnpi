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
#include <data/Request.hpp>
#include <modules/RequestsThread.hpp>


Request::Request(Face* face, Interest* interest, RequestsThread* rt, PendingInterestTable* pit, ForwardingInformationBase* fib)
{
	this->pit = pit;
	this->fib = fib;
	this->face = face;
	this->interest = interest;
	this->rt = rt;
	
}


void Request::expressInterest(void)
{
	this->face->expressInterest(*this->interest,
                           bind(&Request::onData, this,  _1, _2),
                           bind(&Request::onNack, this, _1, _2),
                           bind(&Request::onTimeout, this, _1));

	this->face->processEvents();
}	


void Request::onData(const Interest& interest, const Data& data)
{
	Name name = interest.getName();
	this->fib->insert(&name, this->face);
	this->pit->getMatchingEntry(&name);
	/**
	  * TODO: Put the data on the global face
	  */	
}

void Request::onTimeout(const Interest& interest)
{
	this->rt->decrementRequests();
}

void Request::onNack(const Interest& interest, const lp::Nack& nack)
{
	this->rt->decrementRequests();
}
