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


#include <stdint.h>
#include <stdlib.h>
#include <data/Packet.hpp>

class Link;


class Ethernet
{
    private:
		Link* link;
    public:
		Ethernet(Link* link);
		/**
		  * EFFECTS: process an ethernet frame containing NDN packet
		  * MODIFIES: uint8_t* frame
		  * REQUIRES: the ethernet frame to be carrying NDN packet
		  * PARAMETERS: 
		  * - uint8_t* frame: a raw recieved ethernet frame that contain a NDN packet
		  */
		void packet_process_frame(uint8_t* frame);

};

#endif /*..._ETHERNET_*/