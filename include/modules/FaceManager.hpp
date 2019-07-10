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

#ifndef _MODULES_FACEMANAGER_
#define _MODULES_FACEMANAGER_

#include <queue>
#include <unordered_set>
#include <string>
#include <observer/RequestSubject.hpp>
#include <ndn-cxx/face.hpp>
#include <data/Request.hpp>
#include <data/Interface.hpp>
#include <mutex>

using namespace std;
using namespace ndn;

class FaceManager : public RequestObserver
{
	private:
		mutex currentNamesLock; 
		mutex nackslock;

		unordered_set<string> currentNames;	
		vector<Request*> requests;
		queue<const lp::Nack*> nacks;
		unordered_set<Interface*> interfaces;
        bool hasInterests;
		void expressAllInterests(void);
		void processEventsForAllInterfaces(void);
		void joinAllInterfaces(void);
		void sendNacks(void);
	
	public:

		~FaceManager(void);
		/**
		  * EFFECTS: adds as many interests as necessary in the requests queue according to the
		  * 		 decision of the FIB
		  * MODIFIES: this
		  * REQUIRES: do not call this function on the same interest twice
		  * PARAMETERS:
		  * - Interest interest: the interest to be inserted
		  */
		void addRequest(Interest interest);
		
		/**
		  * EFFECTS: expresses all the interests that were previously added by addRequest 
		  * MODIFIES: this
		  */
		void sendAll(void);
	
		/**
		  * EFFECTS: deletes all the request in the requests queue
		  * MODIFIES: this->requests
		  */
		void deleteAllRequest(void);
	
		void update(RequestSubject* subject);
		void update(RequestSubject* subject, const Data& data);
		void update(RequestSubject* subject, const lp::Nack& nack);


        void onData(const Interest& interest, const Data& data);
		void onTimeout(const Interest& interest);
		void onNack(const Interest& interest, const lp::Nack& nack);
};


#endif /*...MODULES_FACEMANAGER_*/



