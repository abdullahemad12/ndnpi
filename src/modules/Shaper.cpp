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

#include <ndnpi.hpp>
#include <modules/Shaper.hpp>
#include <ndn-cxx/face.hpp>
#include <assert.h>
#include <iostream>

using namespace std;
using namespace ndn;

Shaper::Shaper(unsigned int capacity)
{
	this->capacity = capacity;
	this->terminated = false;
	this->t = NULL;
}

Shaper::~Shaper(void)
{
	if(t != NULL)
	{
		terminated = true;
		t->join();
		delete t;	
	}
}

unsigned int Shaper::calculateCurrentLoad(void)
{
	unsigned int totalSize = 0;
	for(int i = 0; i < N_PRIORITIES; i++)
	{
		totalSize += shaping_queues[i].size();
	}
	return totalSize;
}


void Shaper::forward(void)
{
	while(!terminated)
	{
		lock.lock();
		unsigned int totalSize = calculateCurrentLoad();
		
		if(totalSize > 0)
		{
			/*iterate over all the queues according to the given percentage*/
			calculatePriorityPercentage();
            int leftOvers = 0;
            (void) leftOvers;
			for(int i = 0; i < N_PRIORITIES; i++)
			{
 				int n_packets = capacity * alphas[i];
				for(int j = 0; j < n_packets && !shaping_queues[i].empty(); j++)
				{
					Interest interest = shaping_queues[i].front();
					shaping_queues[i].pop();
					faceManager->addRequest(interest);
                    --n_packets;
				}
                leftOvers = n_packets;

			}
		}
		/* At this point we dont need the shaping queues anymore
         * and faceManager->sendAll() is a blocking function call 
		 * Therefore it's better to release the lock now inorder to allow other threads to
		 * Add the shaping queues
 		 */
		lock.unlock();
		faceManager->sendAll();
	}
}


void Shaper::calculatePriorityPercentage(void)
{
	float acc = 0;

	/*how many queues will the acc be divided on*/
	int n_priorities = N_PRIORITIES;

	for(int i = 0; i < N_PRIORITIES; i++)
	{
		if(shaping_queues[i].size() == 0)
		{
			// distribute the percentage among other weights
			acc += weights[i];
			--n_priorities;	
			alphas[i] = 0;		
		}
		else
		{
			alphas[i] = weights[i];
		}
	}
	
	acc /= n_priorities;
	for(int i = 0; i < N_PRIORITIES; i++)
	{
		if(shaping_queues[i].size() > 0)
		{
			alphas[i] += acc;
		}		
	}
	
	/*just for debugging purpose*/	
	float totalAlphas = 0;	
	for(int i = 0; i < N_PRIORITIES; i++)	
	{
		totalAlphas += alphas[i];
	}
	
	assert(totalAlphas <= 1 && totalAlphas >= 0.98);
}

void Shaper::setWeight(float weight, int i)
{
	assert(i < N_PRIORITIES);
	weights[i] = weight;
}

void Shaper::run(void)
{
	this->t = new thread(bind(&Shaper::forward, this));
}

void Shaper::decreaseCapacity(void){

}

bool Shaper::addInterest(Interest interest)
{
    pitLock.lock();
    bool contains = pit.find(interest.getName().toUri()) != pit.end();
    pitLock.unlock();
    if(contains)
    {
        return true;
    }

	lock.lock();
	bool ret = true;
	
	/*Allow the qeue to store more than the capacity of the link*/
    uint8_t priority = interest.getPriority();
	assert(priority < N_PRIORITIES);
	shaping_queues[priority].push(interest);

	lock.unlock();	

    pitLock.lock();
    pit.insert(interest.getName().toUri());
    pitLock.unlock();
	return ret;
}

void Shaper::removeFromPit(string key)
{
    pitLock.lock();
    pit.erase(key);
    pitLock.unlock();   
}
