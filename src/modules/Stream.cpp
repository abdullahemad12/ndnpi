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

#include <modules/Stream.hpp>
#include <modules/RequestsThread.hpp>
#include <modules/PendingInterestTable.hpp>
#include <modules/ForwardingInformationBase.hpp>
#include <iostream>
#include <data/Interface.hpp>

Stream::Stream(void)
{
	ForwardingInformationBase* fib = new ForwardingInformationBase("rt");
	int capacity = 25; /*should be changed later*/
	shaper = new Shaper(capacity, fib);
	shaper->setWeight(0.50, 0);
	shaper->setWeight(0.25, 1);
	shaper->setWeight(0.15, 2);
	shaper->setWeight(0.10, 3);
	shaper->run();
}

void Stream::onInterest(const InterestFilter& filter, const Interest& interest)
{
	// forward the interest
	if(!shaper->addInterest(interest))
	{
		lp::Nack nack(interest);
		nack.setReason(lp::NackReason::CONGESTION);
		const lp::Nack nack1(nack);
		putNack(nack1);
	}
}

void Stream::listen(void)
{
    this->m_face.setInterestFilter("/",
                         bind(&Stream::onInterest, this, _1, _2),
                         RegisterPrefixSuccessCallback(),
                         bind(&Stream::onRegisterFailed, this, _1, _2));
    this->m_face.processEvents();
}

void Stream::onRegisterFailed(const Name& prefix, const std::string& reason)
{
    std::cerr << "ERROR: Failed to register prefix \""
              << prefix << "\" in local hub's daemon (" << reason << ")"
              << std::endl;
    m_face.shutdown();
	exit(1);
}

void Stream::putData(const Data& data)
{
	 cout << "New data was Retrieved\n";
	 this->m_face.put(data);
}

void Stream::putNack(const lp::Nack& nack)
{
	this->m_face.put(nack);
}

void Stream::decreaseCapacity(void)
{
	this->shaper->decreaseCapacity();
}
