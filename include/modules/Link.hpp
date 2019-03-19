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

#ifndef _MODULES_LINK_
#define _MODULES_LINK_

#include <stdlib.h>
#include <stdint.h>
#include <modules/Ethernet.hpp>
class Link
{
    private:
      int sockfd;
	  Ethernet* eth;
	public:
		Link(int sockfd);
		/**
       	  * void -> void
		  * EFFECTS: listens on the socket and captures incoming ethernet packets 
		  *          then passes them to the Ethernet class for processing
		  * MODIFIES: this
		  * REQUIRES: sockfd to be pointing to a valid file descriptor
		  */
		void listen(void);
		/**
		  * uint8_t* -> void
		  * EFFECTS: sends a given ethernet frame on sockfd
		  * REQUIRES: frame to have a valid ethernet frame format
          */ 
		void transmit(uint8_t* frame);
};

#endif /*..._MODULES_LINK_*/