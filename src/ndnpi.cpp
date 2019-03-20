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

#include <iostream>
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
#include <ndnpi.hpp>
#include <modules/Ethernet.hpp>
#include <modules/Link.hpp>
#include <data/Ethernet.hpp>
using namespace std;

/**
  * Some of the code in the main function is adapted from: 
  * Author: @austinmarton
  * link: https://gist.github.com/austinmarton/1922600
  */
int main(int argc, char* argv[])
{
	int sockfd; /*socket file descriptor*/
	int sockopt;
	struct ifreq ifopts; /* set promiscuous mode */
	char ifName[IFNAMESIZE]; /*interface name*/	
	int err;

	if(argc > 1)	
	{
		strcpy(ifName, argv[1]);	
	}
	else
	{
		strcpy(ifName, DEFAULT_IF);
	}
	
	/*open PF_PACKET socket, listening for Ethernet frames*/
	if ((sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1)
	{
		perror("couldn't listen on this interface");
		return 1;
	}

	/*set interface to promiscuous mode*/
	strncpy(ifopts.ifr_name, ifName, IFNAMSIZ-1);
	ioctl(sockfd, SIOCGIFFLAGS, &ifopts);
	ifopts.ifr_flags |= IFF_PROMISC;
	ioctl(sockfd, SIOCSIFFLAGS, &ifopts);
	
	
	/* Allow the socket to be reused - incase connection is closed prematurely */
	err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof sockopt);
	if(err == -1) 
	{
		perror("setsockopt failed\n");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	/* Bind to device */	
	err = setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, ifName, IFNAMESIZE-1);
	if(err == -1)
	{
		perror("Couldn't bind the socket to the device");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	cout << "The socket was opened successfully\n";


	Link* link = new Link(sockfd);

	link->listen();

	/*exit*/
	close(sockfd);
	exit(0);	
}