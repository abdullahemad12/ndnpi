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

#ifndef _MODULES_FORWARDINGINFORMATIONBASE_
#define _MODULES_FORWARDINGINFORMATIONBASE_
#include <ndn-cxx/face.hpp>
#include <datastructures/linkedlist.h>
#include <vector> 
using namespace std;
using namespace ndn;
class ForwardingInformationBase
{
	private:
		struct linkedlist* faces;
		struct linkedlist* entries;
		/**
		  * EFFECTS: parses the interfaces file and intializes the faces accordingly  
		  * MODIFIES: this 
		  * REQUIRES: the given file to follow this rules:
		  *             ID IP PORT
		  * 		  all the fields are space-sperated
		  * PARAMETERS:
		  * - const char* tpath: the path to the interfaces table
		  */
		void parseTable(const char* tpath);

	public:
		/**
		  * const char* tpath: the path to the routing table
		  */
		ForwardingInformationBase(const char* tpath);
		/**
		  * EFFECTS: given a name performs the lognest prefix match and decides the next hop(s)
		  * RETURNS: vector of faces
		  * PARAMETERS:
		  *  - Name* name: the name to perform the lpm on
		  */ 
		vector<Face*> computeMatchingFaces(Name* name);

		/**
		  * EFFECTS: inserts a new Name entry in the FIB. If the name already exists, then it updates it
		  * REQUIRES: the data associated with that name to be recieved through this face 
		  * PARAMETERS:
		  * - Name* name: the name of the new data
		  * - Face* face: the face of that the data was received on
		  */
		void insert(Name* name, Face* face);

};

#endif /*..._MODULES_FORWARDINGINFORMATIONBASE_*/
