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

#ifndef _OBSERVER_REQUESTSUBJECT_
#define _OBSERVER_REQUESTSUBJECT_

#include <list>
#include <observer/RequestObserver.hpp>
#include <ndn-cxx/face.hpp>

using namespace std;
using namespace ndn;

class RequestSubject
{
	private:
		bool hasChanged;
		list<RequestObserver*> observers;
	public:
		RequestSubject(void);
		void setHasChanged(void);
		void notifyObservers(const Data& data);
		void notifyObservers(void);
		void notifyObservers(const lp::Nack& nack);
};

#endif /*..._OBSERVER_REQUESTSUBJECT_*/
