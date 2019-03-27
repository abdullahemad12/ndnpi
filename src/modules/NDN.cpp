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

#include <modules/NDN.hpp>
#include <ndn-cpp/interest.hpp>
#include <ndn-cpp/data.hpp>

using namespace ndn;

NDN::NDN(void)
{
	this->downstream = new Downstream();
	this->upstream = new Upstream();
}


Packet* NDN::process_packet(uint8_t* data, size_t size)
{
	uint8_t type = *data;
	Packet* packet = NULL;
	switch(type)
	{
		case NDN_INTEREST:
		{
			/*decode the interest packet*/
			Interest* interest = new Interest();
			interest->wireDecode(data, size);

			packet = this->upstream->process_interest_packet(interest);

			delete interest;
			break;
		}
		case NDN_DATA:
		{
			Data* ndn_data = new Data();
			ndn_data->wireDecode(data, size);
			
			packet = this->downstream->process_data_packet(ndn_data);
			delete ndn_data;
			break;
		}
		default: return NULL;
	}
	return packet;
}