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


#ifndef _MODULES_CLASSIFIER_
#define _MODULES_CLASSIFIER_

#include <stdint.h>
#include <ndn-cxx/face.hpp>
#include <vector>

using namespace std;
using namespace ndn;
/*
 * Features that determines the priority of the packet
 * 1. Previously calculated RTT 
 * 2. Freshness 
 * 3. Prefix Traffic frequency
 * 4. Old priority
 */
class Classifier
{
    private:
        uint8_t caclulateRTTFeature(Interest& interest);
        uint8_t calculateFreshnessFeature(Interest& interest);
        uint8_t calculatePrefixTrafficFrequencyFeature(Interest& interest);
        float calculatePriorityAccordingToProbability(Interest& interest, vector<int> priorityList, uint8_t priority, float probability);

    public:
        Classifier(void);
        void classifyInterestPriority(Interest& interest);        
};


#endif /*..._MODULES_CLASSIFIER_*/

