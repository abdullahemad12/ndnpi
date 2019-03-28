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

#include <modules/ContentStore.hpp>
#include <datastructures/hashtable.h>
#include <iostream>
#include <ndn-cpp/data.hpp>

ContentStore::ContentStore(void)
{
	this->ht_store = ht_create(CS_MAX_SIZE);
	/*figure a way to check for ENOMEM*/
}


ContentStore::~ContentStore(void)
{
	ht_destroy(this->ht_store, NULL); /*At this point, some memory leaks on Exit wont cause a lot of harm*/
}


ndn::Data* ContentStore::lookup(ndn::Name& name)
{
	std::string nameStr = name.toUri();

	char* ht_name = (char*)malloc((sizeof(char) * nameStr.size()) + 1);
	for(int i = 0, n = nameStr.size(); i < n; i++)
	{
		ht_name[i] = nameStr[i];
	}

	ndn::Data* data = (ndn::Data*)ht_get(this->ht_store, ht_name);
	

	free(ht_name);

	return data;
}