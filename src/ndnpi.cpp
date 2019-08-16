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

#include <modules/Stream.hpp>
#include <modules/ForwardingInformationBase.hpp>
#include <modules/FaceManager.hpp>
#include <ndnpi.hpp>
#include <iostream>
#include <cstdlib>
#include <MDP/MDP.hpp>

using namespace std;

/*prototypes*/
void init_shaper(int capacity);

Stream* stream; 
ForwardingInformationBase* fib;
Shaper* shaper;
FaceManager* faceManager;
Classifier* classifier;
MDP* mdp;


void exiting(void);

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		cout << "USAGE: ./bin/ndnpi CAPACITY_IN_PACKETS_PER_SECOND\n";
		cout << "EXAMPLE: ./bin/ndnpi 100\n";
		exit(1);
	}

	int capacity = atoi(argv[1]);

	chdir("bin");



    
	/*create the Forwarding information base*/
	fib = new ForwardingInformationBase("rt");
    
    /*create and initialize MDP*/
    mdp = new MDP(fib->getInterfaces().size(), 0.5);

    /*create the packets classifier*/
    classifier = new Classifier();

	/*create a new Stream*/
	stream = new Stream();
	
	/*initialize the FaceManager*/
	faceManager = new FaceManager();

	/*create and initialize the shaper*/
	init_shaper(capacity);
	
	/*on exit signal free all the memory*/
	atexit(exiting);


	/*start Listening on for Interests*/	
	stream->listen();

	
}

void exiting(void)
{
	delete fib;
	delete stream;
	delete faceManager;
	delete shaper;
}

void init_shaper(int capacity)
{
	shaper = new Shaper(capacity);
	
	/*set the weight of each priority*/	
	shaper->setWeight(0.50, 0);
	shaper->setWeight(0.25, 1);
	shaper->setWeight(0.15, 2);
	shaper->setWeight(0.10, 3);
	shaper->run();
}
