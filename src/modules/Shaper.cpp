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

#include <modules/Shaper.hpp>
#include <ndn-cxx/face.hpp>
#include <assert.h>

Shaper::Shaper(unsigned int capacity)
{
	this->capacity = capacity;
}

unsigned int Shaper::calculateCurrentLoad(void)
{
	unsigned int totalSize = 0;
	for(int i = 0; i < N_PRIORITIES; i++)
	{
		totalSize = shaping_queues[i].size();
	}
	return totalSize;
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
	
	assert(totalAlphas <= 1);
}
