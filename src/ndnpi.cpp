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

#include <stdint.h>

uint8_t dst[] = {0x02, 0x42, 0x5c, 0xb7, 0xdf, 0x27};
uint8_t src[] = {0x18, 0x03, 0x73, 0x90, 0xad, 0xe4};


/**
  * Prototypes
  */
int open_reading_socket(char* ifName);
int open_writing_socket(char* if_name, struct sockaddr_ll* sock_addr_ret);

/**
  * Some of the code in the main function is adapted from: 
  * Author: @austinmarton
  * link: https://gist.github.com/austinmarton/1922600
  */
int main(int argc, char* argv[])
{
	char ifName[IFNAMESIZE]; /*interface name*/	
	int rsockfd;
	int wsockfd;
	
	if(argc > 1)	
	{
		strcpy(ifName, argv[1]);	
	}
	else
	{
		strcpy(ifName, DEFAULT_IF);
	}
	
	rsockfd = open_reading_socket(ifName);

	struct sockaddr_ll sock_addr;

	wsockfd = open_writing_socket(ifName, &sock_addr);

	uint8_t twelve = 12;

	data::Ethernet* eth = new data::Ethernet(dst, src, &twelve, sizeof(uint8_t));
	
	uint8_t* packet;
	size_t size = eth->encapsulate(&packet);




	Link* link = new Link(rsockfd, wsockfd, sock_addr);
	link->transmit(packet, size);
	link->listen();

	/*exit*/
	close(rsockfd);
	exit(0);	
}

int open_writing_socket(char* if_name, struct sockaddr_ll* sock_addr_ret)
{
	int sock;
  	int if_index;
  	uint8_t if_addr[ETH_ALEN];
	sock = socket (AF_PACKET, SOCK_RAW, htons (ETHER_TYPE));
  	if (sock < 0)
	{
    	perror ("socket()");
		exit(1);
	}

	struct ifreq ifr;

    memset (&ifr, 0, sizeof (ifr));
    strncpy (ifr.ifr_name, if_name, IFNAMSIZ - 1);

    if (ioctl (sock, SIOCGIFINDEX, &ifr) < 0)
	{
    	perror ("SIOCGIFINDEX");
		exit(1);	
    }
	if_index = ifr.ifr_ifindex;

    if (ioctl (sock, SIOCGIFHWADDR, &ifr) < 0)
	{  
    	perror ("SIOCGIFHWADDR");
		exit(1);
    }
	memcpy (if_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);

    struct sockaddr_ll sock_addr;
	sock_addr.sll_ifindex = if_index;
    sock_addr.sll_halen = ETH_ALEN;
    memcpy (sock_addr.sll_addr, dst, ETH_ALEN);
	
	*sock_addr_ret = sock_addr;
	return sock;
}

int open_reading_socket(char* ifName)
{
	int err;
	int sockfd; /*socket file descriptor*/
	int sockopt;
	struct ifreq ifopts; /* set promiscuous mode */



	/*open PF_PACKET socket, listening for Ethernet frames*/
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETHER_TYPE))) == -1)
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

	
	return sockfd;


}

