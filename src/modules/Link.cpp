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
#include <stdint.h>
#include <modules/Link.hpp>
#include <modules/Ethernet.hpp>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <iostream>
#include <data/Ethernet.hpp>

using namespace std;

Link::Link(int sockfd)
{
	this->sockfd = sockfd;
	this->eth = new Ethernet(this);
}


void Link::listen(void)
{
	uint8_t* buf;
	size_t numbytes;

	buf = (uint8_t*)malloc(sizeof(uint8_t) * BUFSIZE);
	if(buf == NULL)
	{
		printf("Couldn't allocate memory for the buffer\n");
		exit(1);
	}
	while(1)
	{
		numbytes = recvfrom(this->sockfd, buf, BUFSIZE, 0, NULL, NULL);
		cout << numbytes;
		cout << "\n";

		data::Ethernet* frame = new data::Ethernet(buf, numbytes);

		uint8_t* payload = frame->extract_payload();
		cout << payload[0];
		cout << "\n";
		if(payload[0] == 12)
		{
			cout << "Recieved number 12 successfully\n";
		}
	}

}


void Link::transmit(uint8_t* frame, size_t size)
{
	write(this->sockfd, frame, size);	
}