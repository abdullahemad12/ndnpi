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

#ifndef _MODULES_SHAPER_
#define _MODULES_SHAPER_


#include <queue>
#include <mutex>
#include <thread>
#include <ndn-cxx/face.hpp>
#include <modules/ForwardingInformationBase.hpp>
#include <unordered_set>

#define N_PRIORITIES 4


using namespace std;
using namespace ndn;

class Shaper
{
	private:
		thread* t; /*thread that forwards the interests*/
		mutex lock;
		mutex capacityLock;

		unsigned int capacity; /*The capacity of this router expressed in Packets/second*/
		queue<Interest> shaping_queues[N_PRIORITIES];
		float weights[N_PRIORITIES];
		float alphas[N_PRIORITIES];
		bool terminated;


		/**
		  * EFFECTS: calculates the sum of all the queues size
		  * RETURNS: the total size of all the queues
		  */
		unsigned int calculateCurrentLoad(void);

		/**
		  * EFFECTS: forwards the packets to their destination while considering 
		  *          the priority of each packet
		  * MODIFIES: this
		  */
		void forward(void);

		/**
		  * EFFECTS: calculates the alphas given the initial weights 
		  *          rule: 
          *            alphas[i] = weight[i] if shaping_queues[i] is not empty
		  *            alphas[i] = 0         if shaping_queues[i] is empty
          * MODIFIES: alphas
	      * REQUIRES: sum weights = 1
		  */  
		void calculatePriorityPercentage(void);
	public:

		Shaper(unsigned int capacity);

		~Shaper(void);
		
		/**
		  * Synchronized
		  * EFFECTS: adds interest to the relevant queue according to it's priority
		  * MODIFIES: this
		  * REQUIRES: interest to have priority < N_PRIORITIES. 
		  * RETURNS: true if the packet was packet was inserted successfully in the queue
		  *          returns false if the full capacity was reached and the packet has to be dropped
		  */
		bool addInterest(Interest interest);
		
		/**
		  * EFFECTS: spawns the thread that keeps forwarding packets
		  */
		void run(void);

		/**
		  * Synchronized
		  * EFFECTS: sets the ith weight value
		  * MODIFIES: this 
		  * REQUIRES: 0 < i < N_PRIORITY
		  */
		void setWeight(float weight, int i);

		/**
		  * Synchronized
		  * EFFECTS: should be called on a congestion NACK. It will divide the current capacity by 2
		  * MODIFIES: this->capacity
		  */
		void decreaseCapacity(void);



};


#endif /*..._MODULES_SHAPER_*/

