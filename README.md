# NDNPI 

NDN Router implementation for RaspberryPi boards.


# Prerequisite 

In order to compile and run this program on any device, the following libraries should be installed with their dependencies: 

1. [NDN C++ library with eXperimental eXtensions](https://github.com/named-data/ndn-cxx)
2. [Named Data Networking Forwarding Daemon](https://github.com/named-data/NFD)
3. Build essential library e.g: `sudo apt-get install build-essential`

# Compilation Guide

To compile this program from a Mac or a linux machine, 

1. clone or download this repository to your local device. 
2. Open the terminal and run `cd path/to/ndnpi/directory`
3. in the terminal run make


# Running NDNPi 

to run NDNPi you need to have a routing file in the bin/ directory called rt. The following section explains the rt routing file format with an example.

1. Create a text file in the /bin directory and call it rt
2. Make sure the nfd is up and running by running `nfd-start`
3. from the main directory run ./bin/ndnpi n where n is a posittive integer number denoting the approximate capacity of this link as Interests/seconds 

# rt Routing file

The routing file contains important static information about the subnetwork the router is currently.
1. the routing file should list all the devices in the subnetwork. 
2. the routing file should list all the edges between devices in the network
3. the routing file should list the data producers in the subnetwork with their prefixes or the edge nodes where data prefixes could be reached through.

## Example: 

```
11
10.42.0.1 6363
10.42.0.27 6363
10.42.0.93 6363
10.42.0.132 6363
10.42.0.145 6363
10.42.0.155 6363
10.42.0.165 6363
10.42.0.221 6363
10.42.0.222 6363
10.42.0.243 6363
10.42.0.244 6363
0
12
0 1 25
0 2 15
1 3 23
2 3 65
3 4 44
3 6 30
2 7 35
7 8 32
8 10 55
4 5 10
4 9 12
6 8 43
3
5 /ndn/examples/testApp/
9 /ndn/examples/cats/melody/
10 /ndn/git/hub
```

1. The first line contains the total number of devices n in the subnetwork.
2. The following n lines contain the IP addresses and ports of the devices.
3. The following line contains the number of edges m in the graph
4. The following m lines contain the edges in the graph 
5. The following line contains the number of producers in the subnetwork K. 
6. the Following k lines contains two spereted information the id of the producer and the prefix it produces. 


# Future Work 

For a more realistic design, the system should make use of the NLSR to fetch information of the network topology instead of reading it from a static file. 

