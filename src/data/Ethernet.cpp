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

#include <arpa/inet.h>
#include <data/Ethernet.hpp>
#include <string.h>
using namespace data;

Ethernet::Ethernet(uint8_t* dpa, uint8_t* spa, uint8_t* data, size_t data_size)
{
	memcpy(this->dpa, dpa, ETHER_ADDR_LEN);
	memcpy(this->spa, spa, ETHER_ADDR_LEN);
	uint8_t* payload = (uint8_t*)malloc(sizeof(uint8_t) * data_size);
	memcpy(payload, data, data_size);
	this->data = payload;
	this->size = data_size;
}

Ethernet::Ethernet(uint8_t* frame, size_t size)
{
	struct eth_hdr* hdr = (struct eth_hdr*) frame;
		
	uint8_t* data = frame + sizeof(struct eth_hdr);
	size_t data_size = size - sizeof(struct eth_hdr);

	memcpy(this->dpa, hdr->ether_dhost, ETHER_ADDR_LEN);
	memcpy(this->spa, hdr->ether_shost, ETHER_ADDR_LEN);
	uint8_t* payload = (uint8_t*)malloc(sizeof(uint8_t) * data_size);

	
	memcpy(payload, data, data_size);
	this->data = payload;	
	this->size = data_size;
	
}


uint8_t* Ethernet::extract_payload(void)
{
	return data;
}

size_t Ethernet::encapsulate(uint8_t** packet)
{
	uint8_t* frame = (uint8_t*) malloc(sizeof(eth_hdr) + this->size);
	if(frame == NULL)
	{
		return 0;
	}

	struct eth_hdr* hdr = (struct eth_hdr*) frame;
	memcpy(hdr->ether_dhost, this->dpa, ETHER_ADDR_LEN);
	memcpy(hdr->ether_shost, this->spa, ETHER_ADDR_LEN);
	hdr->ether_type = htons(ETHER_TYPE);
	
	uint8_t* data = frame + sizeof(struct eth_hdr);

	memcpy(data, this->data, this->size);
	
	*packet = frame;
	
	return sizeof(struct eth_hdr) + this->size;
}