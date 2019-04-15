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

#ifndef _MODULES_PENDINGINTERESTTABLE_
#define _MODULES_PENDINGINTERESTTABLE_

#include <ndn-cxx/face.hpp>
#include <datastructures/hashtable.h>
#include <data/PITEntry.hpp>
using namespace ndn;
class PendingInterestTable
{
	private:
		struct hashtable* entries;
	public:
		PendingInterestTable(void);
		/**
		  * EFFECTS: inserts the given interest in the PIT
		  * MODIFIES: this
		  * RETURNS: false if the same interest with the same nonce was found
		  */
		bool insert(Interest* interest);
		/**
		  * EFFECTS: gets the PIT entry with the matching name and removes it
		  * MODIFIES: this 
		  * RETURNS: the PITEntry
		  */
		PITEntry* getMatchingEntry(Name* name);
		/**
		  * EFFECTS: removes the PIT entry with the matching name from the PIT
		  * MODIFIES: this
		  */
		void remove(Name* name);
};

#endif /*..._MODULES_PENDINGINTERESTTABLE_*/
