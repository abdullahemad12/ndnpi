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

#include <modules/Classifier.hpp>
#include <vector>
#include <ndnpi.hpp>

using namespace std;
using namespace ndn;


/**
  * prototypes
  */
int calculateOcc(vector<int> priorityList, int target);

Classifier::Classifier(void)
{
    
}

void Classifier::classifyInterestPriority(Interest& interest)
{
    float probability = 0;
  
    vector<int> priorityList;
    priorityList.push_back(caclulateRTTFeature(interest));
    priorityList.push_back(calculateFreshnessFeature(interest));
    priorityList.push_back(calculatePrefixTrafficFrequencyFeature(interest));
    priorityList.push_back(interest.getPriority());
    

   probability = calculatePriorityAccordingToProbability(interest, priorityList, 0, probability);
   probability = calculatePriorityAccordingToProbability(interest, priorityList, 1, probability);
   probability = calculatePriorityAccordingToProbability(interest, priorityList, 2, probability);
   calculatePriorityAccordingToProbability(interest, priorityList, 3, probability);
    
}

uint8_t Classifier::caclulateRTTFeature(Interest& interest)
{
    float min = fib->getMinimumRTT();
    float max = fib->getMaximumRTT();
    float range = max - min;

    float div = range / 4.0; 
    float rtt = fib->getLPMRtt(interest.getName());

    for(int i = 0; i < 4; i++)
    {
        if(min + (i * div) <= rtt && rtt <= min + ((i + 1) * div))
        {
            return 3 - i;
        }
    }
    return 2;/*not found*/

}
uint8_t Classifier::calculateFreshnessFeature(Interest& interest)
{
    return interest.getMustBeFresh() ? 1 : 2;
}
uint8_t Classifier::calculatePrefixTrafficFrequencyFeature(Interest& interest)
{
    int min = fib->getMinimumFrequency();
    int max = fib->getMaximumFrequency();
    float range = max - min;

    float div = range / 4.0;

    int freq = fib->getLPMFrequency(interest.getName());
    for(int i = 0; i < 4; i++)
    {
        if(min + (i * div) <= freq && freq <= min + ((i + 1) * div))
        {
            return 3 - i;
        }
    }

    
    return 2; /*not found*/
}


float Classifier::calculatePriorityAccordingToProbability(Interest& interest, vector<int> priorityList,
                                                                 uint8_t priority, float probability)
{
    int occ = calculateOcc(priorityList, priority);
    float curProb = (float) occ / (float) priorityList.size();

    if(curProb >= probability) 
    {
        probability = curProb;
        interest.setPriority(priority);
    }

    return probability;
}

/**************************
 *     static Helpers     *
 **************************/

int calculateOcc(vector<int> priorityList, int target)
{
    int count = 0;
    for(unsigned int i = 0; i < priorityList.size(); i++)
    {
        if(priorityList[i] == target)
        {
            count = count + 1 + i;
        }
    }
    return count;
}
