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
#include <bits/stdc++.h> 
#include <string>
#include <observer/RequestSubject.hpp>
#include <ndn-cxx/face.hpp>
#include <data/Request.hpp>

using namespace std;
using namespace ndn;

class FaceManager : public RequestSubject
{
	private:
		unordered_set<string> currentNames;	
		queue<Request> requests;
		queue<const lp::Nack> nacks;
		unordered_set<Face*> faces;

	public:
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
};


#endif /*...MODULES_FACEMANAGER_*/



