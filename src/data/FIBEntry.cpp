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

#include <data/FIBEntry.hpp>
#include <data/Interface.hpp>
#include <ndn-cxx/face.hpp>

using namespace ndn;

FIBEntry::FIBEntry(Name name, Interface* interface, float rtt)
{
	this->name = name;
	this->interface = interface;
	this->rtt = rtt;
    this->frequency = 1;
}



FIBEntry::~FIBEntry(void)
{

}

const Name FIBEntry::getName(void)
{
	return (const Name) this->name;
}

Interface* FIBEntry::getInterface(void)
{
	return this->interface;
}


float FIBEntry::getRtt(void)
{
	return this->rtt;
}

void FIBEntry::setRtt(float rtt)
{
	this->rtt = rtt;
}

void FIBEntry::incrementFrequency(void)
{
    ++this->frequency;
}

int FIBEntry::getFrequency(void)
{
    return this->frequency;
}
