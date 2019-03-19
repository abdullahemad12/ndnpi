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

#ifndef _MODULES_ETHERNET_
#define _MODULES_ETHERNET_

#define ETHER_TYPE	0x0800

#include <stdint.h>
#include <stdlib.h>

typedef struct eth_hdr
{
	/*attribute packed*/
} eth_hdr_t;

class Ethernet
{
    private:

    public:
		Ethernet(void);
		/**
          * uint8_t* -> bool
		  * EFFECTS: makes sure the given buffer contains a valid ethernet frame
		  * RETURNS: true if buf contains a valid ethernet frame, false otherwise 
		  * PARAMETERS:
		  * - uint8_t* buf: the buffer
          */
        bool validate(uint8_t* buf);
		/**
		  * uint8_t* -> uint8_t*
 		  * EFFECTS: extracts the payload from the ethernet frame
		  * RETURNS: pointer to the first byte of the ethernet payload
          * PARAMETERS:
		  * - uint8_t* frame: the ethernet frame to be modified
		  */
        uint8_t* extract_payload(uint8_t* frame);
		/**
		  * uint8_t*, size_t ->uint8_t*
		  * EFFECTS: puts the given data in an Ethernet frame
		  * RETURNS: pointer to first byte of the new ethernet frame
		  * PARAMETERS:
		  * - uint8_t* payload: pointer to the first byte of the payload
		  * - size_t: the size of the payload in bytes
		  */
        uint8_t* encapsulate(uint8_t* payload, size_t n);
};

#endif /*..._ETHERNET_*/