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


#ifndef _DATA_INTERFACE_
#define _DATA_INTERFACE_
#include <ndn-cxx/face.hpp>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>
using namespace std;
using namespace ndn;

/**
  * Source: https://riptutorial.com/cplusplus/example/30142/semaphore-cplusplus-11
  */
class Semaphore {
public:
    Semaphore (int count_ = 0)
    : count(count_) 
    {
    }
    
    inline void notify( ) {
        std::unique_lock<std::mutex> lock(mtx);
        count++;
        //notify the waiting thread
        cv.notify_one();
    }
    inline void wait() {
        std::unique_lock<std::mutex> lock(mtx);
        while(count == 0) {
            cv.wait(lock);
        }
        count--;
    }
private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
};
class Interface
{
	private:
        int id;
		string ip;
		string port;
		Face* face;
		void t_func(void);
		thread* t;
		/*additional memory to handle failures gracefully*/
		vector<NackCallback> afterNackeds;
		vector<Interest> interests;
        Semaphore* sem;
	public:
		Interface(int id, string ip, string port);
		~Interface(void);
		string getIp(void);
		string getPort(void);
		int getId(void);
		void expressInterest(const Interest &interest, const DataCallback &afterSatisfied, 
							 const NackCallback &afterNacked, const TimeoutCallback &afterTimeout);
		void processEvents(void);
        void connectToFace(void);
};


#endif /*..._DATA_INTERFACE*/
