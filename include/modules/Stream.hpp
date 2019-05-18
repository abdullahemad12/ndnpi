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

#ifndef _MODULES_STREAM_
#define _MODULES_STREAM_

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>

#include <modules/PendingInterestTable.hpp>
#include <modules/ForwardingInformationBase.hpp>
#include <modules/Shaper.hpp>


class Stream
{
	private:
		Face m_face;
		KeyChain m_keyChain;
		Shaper* shaper;

		void onInterest(const InterestFilter& filter, const Interest& interest);
		void onRegisterFailed(const Name& prefix, const std::string& reason);
	public:
		Stream(void);	
		void listen(void);
		void putData(const Data& data);
};



#endif /*_MODULES_STREAM_*/
