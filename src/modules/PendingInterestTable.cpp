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

#include <ndn-cxx/face.hpp>
#include <modules/PendingInterestTable.hpp>
#include <data/PITEntry.hpp>
#include <datastructures/hashtable.h>
#include <iostream>
using namespace std;
PendingInterestTable::PendingInterestTable(void)
{
	this->entries = ht_create(10000);
	if(this->entries == NULL)
	{
		cout << "Insufficient Memory\n";
		exit(2);
	}
	
}

bool PendingInterestTable::insert(Interest* interest)
{
	PITEntry* entry = (PITEntry*) ht_get(this->entries, (char*)interest->getName().toUri().c_str());
	if(entry == NULL)
	{
		entry = new PITEntry(interest);
		ht_put(this->entries, (char*)interest->getName().toUri().c_str(), interest);
	}
	else
	{
		return entry->addNonce(interest->getNonce());
	}
	return true;

}

PITEntry* PendingInterestTable::getMatchingEntry(Name* name)
{
	char* namec =  (char*)name->toUri().c_str(); 
	 return (PITEntry*) ht_remove(this->entries, namec);
}
