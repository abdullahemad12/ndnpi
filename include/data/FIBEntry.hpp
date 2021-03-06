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

#ifndef _DATA_FIBENTRY_
#define _DATA_FIBENTRY_

#include <ndn-cxx/face.hpp>
#include <data/Interface.hpp>

using namespace ndn;

class FIBEntry
{
	private: 
		Name name;
		Interface* interface;
		float rtt; /*expected RTT for this name when forwarded on this interface*/
        int frequency;
	public:
		FIBEntry(Name name, Interface* interface, float rtt);
		~FIBEntry(void);
		const Name getName(void);
		Interface* getInterface(void);
		float getRtt(void);
		void setRtt(float rtt);
        void incrementFrequency(void);
        int getFrequency(void);
};



#endif /*..._DATA_FIBENTRY_*/

