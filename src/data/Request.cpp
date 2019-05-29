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
#include <ndnpi.hpp>
#include <data/Interface.hpp>
#include <chrono>
#include <string>

using namespace std;

Request::Request(Interest interest,  Interface* interface) : RequestSubject()
{
	this->interface = interface;	
	this->interest = interest;
	this->name = interest.getName();
}

Request::~Request(void)
{

}

void Request::expressInterest(void)
{
	interface->expressInterest(interest,
                           bind(&Request::onData, this,  _1, _2),
                           bind(&Request::onNack, this, _1, _2),
                           bind(&Request::onTimeout, this, _1));

	start = std::chrono::steady_clock::now();
}	


void Request::onData(const Interest& interest, const Data& data)
{
	end = std::chrono::steady_clock::now();
	setHasChanged();
	notifyObservers(data);
	
}

void Request::onTimeout(const Interest& interest)
{
	setHasChanged();
	notifyObservers();
}

void Request::onNack(const Interest& interest, const lp::Nack& nack)
{
	if(IS_CONGESTION_NACK(nack)){
		shaper->decreaseCapacity();	
	}
	setHasChanged();
	notifyObservers(nack);
}

Interest Request::getInterest(void)
{
	return this->interest;
}

string Request::getInterestNameUri()
{
	return name.toUri();
}

Interface* Request::getInterface(void)
{
	return interface;
}

float Request::calculateRtt(void)
{
	const auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
	float time = duration.count() / 1000.0 ;
	return time;
}	
