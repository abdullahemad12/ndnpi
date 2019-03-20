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

#define ETHER_TYPE	0x0BCB

#ifndef _DATA_ETHERNET_
#define _DATA_ETHERNET_




#include <stdint.h>
#include <stdlib.h>
/*
 * This will be used to extract header data out of Ethernet frame
 */
struct eth_hdr
{
	#ifndef ETHER_ADDR_LEN
	#define ETHER_ADDR_LEN 6
	#endif
    uint8_t  ether_dhost[ETHER_ADDR_LEN];    /* destination ethernet address */
    uint8_t  ether_shost[ETHER_ADDR_LEN];    /* source ethernet address */
    uint16_t ether_type;                     /* packet type ID */
} __attribute__ ((packed)) ;

typedef struct eth_hdr eth_hdr_t;


namespace data
{
	class Ethernet
	{
		private:
			uint8_t dpa[ETHER_ADDR_LEN]; /*destination physical address*/
			uint8_t spa[ETHER_ADDR_LEN]; /*source physical address*/
			uint8_t* data;
			size_t size;
		public:
		/**
 		  * NOTE: the size is the size of the whole frame
		  */
		Ethernet(uint8_t* frame, size_t size);

		/**
		  * EFFECTS: creates a new Ethernet object
		  * REQUIRES: dpa and spa to be 6 bytes in size and in host byte order
		  * MODIFIES: this
		  */
		Ethernet(uint8_t* dpa, uint8_t* spa, uint8_t* data, size_t data_size);
		/**
		  * void -> uint8_t*
 		  * EFFECTS: extracts the payload from the ethernet frame
		  * RETURNS: pointer to the first byte of the ethernet payload
		  */
        uint8_t* extract_payload(void);
		/**
		  * uint8_t** -> size_t
		  * EFFECTS: puts the given data in an Ethernet frame
		  * RETURNS: pointer to first byte of the new ethernet frame
		  */
        size_t encapsulate(uint8_t** packet);
	};
};




#endif /*...._DATA_ETHERNET_*/