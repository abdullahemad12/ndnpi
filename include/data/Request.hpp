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

#ifndef _DATA_REQUEST_
#define _DATA_REQUEST_

#include <ndn-cxx/face.hpp>
#include <modules/PendingInterestTable.hpp>
#include <modules/ForwardingInformationBase.hpp>
#include <data/Interface.hpp>
#include <observer/RequestSubject.hpp>
#include <chrono>
#include <ctime>
#include <ratio>
#include <chrono>
#include <string>

#define IS_CONGESTION_NACK(nack) nack.getReason() == lp::NackReason::CONGESTION

using namespace std;

class Request : public RequestSubject
{
	private:
		chrono::steady_clock::time_point start;
		chrono::steady_clock::time_point end;
		Interest interest;
		Name name;
		Interface* interface;
	public:
		Request(Interest interest, Interface* interface);
		~Request(void);
	
		void expressInterest(void);	
		Interest getInterest(void);
		void onData(const Interest& interest, const Data& data);
		void onTimeout(const Interest& interest);
		void onNack(const Interest& interest, const lp::Nack& nack);
		string getInterestNameUri(void);
		Interface* getInterface(void);
		float calculateRtt(void);
};




#endif /*..._DATA_REQUEST_*/
