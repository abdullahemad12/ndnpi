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
#include <data/Interface.hpp>
#include <data/Request.hpp>
#include <data/FIBEntry.hpp>
#include <unordered_map>
#include <mutex>


class Request;

using namespace std;
using namespace ndn;
class ForwardingInformationBase
{
	private:
		mutex entriesLock;
		vector<Interface*> interfaces;
		unordered_map<string, vector<FIBEntry*>> entries;

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

		/**
		  * EFFECTS: initializes the score map for the computeMatchingFaces function
		  * RETURNS: the unordered_map
		  */
		unordered_map<Interface*, int> initScoreMap(void);
	
		/**
		  * EFFECTS: computes the score for all the interfaces according to the 
		  * 		 longest prefix match
		  * MODIFIES: scoreMap
		  * PARAMETERS:
		  * unordered_map& scoreMap: the score map to be modified
		  */ 
		void computeScores(unordered_map<Interface*, int>& scoreMap, const Name& name);

		/**
		  * EFFECTS: Checks if it is the first time to forward this name or not
		  * RETURNS: true if it is the first time, false otherwise
		  */
		bool isFirstForward(unordered_map<Interface*, int>& scoreMap);


		/**
		  * EFFECTS: Checks if it is the first time to forward this name or not
		  * RETURNS: true if it is the first time, false otherwise
		  */
		vector<Interface*> calculateFinalSetAccordingToPriority(vector<Interface*>& intarr, uint8_t priority);

		/**
		  * EFFECTS: sorts the interfaces according to the score of each one 
		  * RETURNS: list of sorted interfaces
		  */
		vector<Interface*> sortInterfaces(unordered_map<Interface*, int>& scoreMap);

	public:
		/**
		  * const char* tpath: the path to the routing table
		  */
		ForwardingInformationBase(const char* tpath);
		
		~ForwardingInformationBase(void);
		/**
		  * EFFECTS: given a name performs the lognest prefix match and decides the next hop(s)
		  *			 default behaviour if the name does not match any prefix or if the FIB is empty
		  *			 is to return a list of all the interfaces
		  * RETURNS: vector of faces
		  * PARAMETERS:
		  *  - Name* name: the name to perform the lpm on
		  */ 
		vector<Interface*> computeMatchingFaces(const Interest& name);

		/**
		  * EFFECTS: inserts a new Name entry in the FIB. If the name already exists, then it updates it
		  * REQUIRES: the data associated with that name to be recieved through this face 
		  * PARAMETERS:
		  * - Name* name: the name of the new data
		  * - Face* face: the face of that the data was received on
		  */
		void insert(Request& request);

		/**
		  * EFFECTS: removes the FIBEntry related to this request's name if it exists
		  * REQUIRES: an NACK for this name to be recieved from the associated interface
		  * Note: this is synchronized. make sure you are not holding any locks while calling it
		  * 	  to avoid deadlocks and program crash 
		  * PARAMETERS:
		  * Request& request: the request whose FIBEntry will be removed
		  */
		void remove(string key, Interface* interface);

		/**
          * EFFECTS: gets the linkedlist of faces 
          * NOTE: Never delete, modify or write to this linkedlist or its elements 
		  *       Only use for reading
 		  */ 
		vector<Interface*> getInterfaces(void);


};

#endif /*..._MODULES_FORWARDINGINFORMATIONBASE_*/
