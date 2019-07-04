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

#include <modules/ForwardingInformationBase.hpp>
#include <ndn-cxx/face.hpp>
#include <datastructures/linkedlist.h>
#include <vector>
#include <iostream>
#include <data/FIBEntry.hpp> 
#include <data/Request.hpp>
#include <unordered_map>
#include <fstream>

using namespace std;
/**
  * Prototypes for helper functions
  */
static int computeLongestCommonPrefixSize(const Name& name1, const Name& name2);


/**
  * EFFECTS: gets the minimum of the two given numbers
  * RETURNS: the mininmum
  * PARAMTERS: 
  * - int x: the first number to be compared 
  * - int y: the second number to be compared
  */
static size_t min(size_t x, size_t y) { return x < y ? x : y;}



/**
  * EFFECTS: parses the interfaces file and intializes the faces accordingly  
  * MODIFIES: this 
  * REQUIRES: the given file to follow this rules:
  *             ID IP PORT
  * 		  all the fields are space-sperated
  * PARAMETERS:
  * - const char* tpath: the path to the interfaces table
  */
void ForwardingInformationBase::parseTable(string tpath)
{
    
    std::ifstream file(tpath);
    if (file.fail()) 
    {
        cout << "Couldn't open the routing table file\n";
		exit(1);  
    }
	
    int nInterfaces; 
    file >> nInterfaces;
    
    vector<Interface*> interfaces;
    for(int i = 0; i < nInterfaces; i++)
    {
        string ip;
        string port;
        file >> ip >> port;
        Interface* interface = new Interface(ip, port);
        interfaces.push_back(interface);
    }

    int source_node;
    file >> source_node;

    int nEdges;
    file >> nEdges;


    int** graph_edges = new int*[nEdges];
    for(int i = 0; i < nEdges; i++)
    {
        graph_edges[i] = new int[3];
    }

    for(int i = 0; i < nEdges; i++)
    {
        file >> graph_edges[i][0] >> graph_edges[i][1] >> graph_edges[i][2];        
    }

    graph = new Graph(graph_edges, interfaces.size(), nEdges, source_node);

    for(int i = 0; i < nEdges; i++)
    {
        delete graph_edges[i];
    }
    delete graph_edges;
    
    int nPrefixes;

    file >> nPrefixes;

    string* prefixes = new string[nPrefixes];
    int* nodeIds = new int[nPrefixes];
    
    for(int i = 0; i < nPrefixes; i++)
    {
        file >> nodeIds[i] >> prefixes[i];
    }
    
    delete nodeIds;
    file.close();

}

ForwardingInformationBase::ForwardingInformationBase(string tpath)
{
	this->parseTable(tpath);
}

ForwardingInformationBase::~ForwardingInformationBase(void)
{
	for(auto& list : entries)
	{
		for(unsigned int i = 0; i < list.second.size(); i++)
		{
			auto& item = list.second[i];
			delete item;
		}
	}
	for(Interface* interface : interfaces)
	{
		delete interface;
	}
}

vector<Interface*> ForwardingInformationBase::computeMatchingFaces(const Interest& interest)
{
	const Name& name = interest.getName();
	vector<Interface*> interfaces;

	/*initialize the score Map*/
	unordered_map<Interface*, int> scoreMap = initScoreMap();
	

	/**
	  * Priority 1 will be forwarded to all Interfaces
	  * Priority 2 will be forwarded to all the interfaces except 2
	  * and so On
      */	
	computeScores(scoreMap, name);

	if(isFirstForward(scoreMap))
	{
		/*I have to broadcast regardless of the priority because I dont know which interface
		  will fetch the data back for sure*/
		return this->interfaces;
	}

    return this->interfaces;
	
	//vector<Interface*> intarr = sortInterfaces(scoreMap);
	
	//interfaces = calculateFinalSetAccordingToPriority(intarr, interest.getPriority());
	
	//return interfaces;
}



void ForwardingInformationBase::insert(Request& request)
{
	string key = request.getInterestNameUri();
	
	Name name = request.getInterest().getName();
	Interface* interface = request.getInterface();
	float rtt = request.calculateRtt();
	
	entriesLock.lock();
	if(entries.find(key) == entries.end())
	{
		vector<FIBEntry*> list;	
		FIBEntry* entry = new FIBEntry(name, interface, rtt);
		list.push_back(entry);
		entries[key] = list;
	}
	else
	{
		vector<FIBEntry*>& vec = entries[key];
		FIBEntry* entry = NULL;		
		for(unsigned int i = 0; i < vec.size(); i++)
		{
			if(vec[i]->getInterface() == interface)
			{	
				entry = vec[i];
			}
		}
		if(entry == NULL)
		{
			entry = new FIBEntry(name, interface, rtt);
			vec.push_back(entry);
		}
		else
		{
			entry->setRtt(rtt);
            entry->incrementFrequency(); 
		}
	}
	entriesLock.unlock();
}

void ForwardingInformationBase::remove(string key, Interface* interface)
{
	entriesLock.lock();
	if(entries.find(key) != entries.end())
	{
		vector<FIBEntry*>& vec = entries[key];
		for(unsigned int i = 0; i < vec.size(); i++) 
		{
			if(vec[i]->getInterface() == interface)
			{	
				FIBEntry* entry = vec[i];
				vec.erase(vec.begin() + i);
				delete entry;
			}
		}
		if(vec.empty())
		{
			entries.erase(key);
		}
	}
	entriesLock.unlock();
}

vector<Interface*> ForwardingInformationBase::getInterfaces(void)
{
	return interfaces;
}

int ForwardingInformationBase::getMinimumFrequency(void)
{
    int min = INT_MAX;
    for(auto& list : entries){
        for(unsigned int i = 0; i < list.second.size(); i++)
        {
            FIBEntry* entry = list.second[i];
            if(entry->getFrequency() < min)
            {
               min = entry->getFrequency();
            }
        }    
    }
    return min;
}

int ForwardingInformationBase::getMaximumFrequency(void)
{
    int max = INT_MIN;
    for(auto& list : entries){
        for(unsigned int i = 0; i < list.second.size(); i++)
        {
            FIBEntry* entry = list.second[i];
            if(entry->getFrequency() > max)
            {
               max = entry->getFrequency();
            }
        }    
    }
    return max;
}


float ForwardingInformationBase::getMaximumRTT(void)
{
    float max = LONG_MIN;
    for(auto& list : entries){
        for(unsigned int i = 0; i < list.second.size(); i++)
        {
            FIBEntry* entry = list.second[i];
            if(entry->getRtt() > max)
            {
               max = entry->getRtt();
            }
        }    
    }
    return max;
}



float ForwardingInformationBase::getMinimumRTT(void)
{
    float min = LONG_MAX;
    for(auto& list : entries){
        for(unsigned int i = 0; i < list.second.size(); i++)
        {
            FIBEntry* entry = list.second[i];
            if(entry->getRtt() < min)
            {
               min = entry->getRtt();
            }
        }    
    }
    return min;
}

int ForwardingInformationBase::getLPMFrequency(const Name& name)
{
    int maxScore = 0;   
    FIBEntry* finalEntry = NULL;
    for(auto& list : entries)
	{
		for(unsigned int i = 0; i < list.second.size(); i++)
		{
			FIBEntry* entry = list.second[i];
            int curScore = computeLongestCommonPrefixSize(name, entry->getName()); 
            if(curScore > maxScore) 
            {
                maxScore = curScore;
                finalEntry = entry;
            }
        }
    }

    if(finalEntry == NULL)
    {
        return INT_MAX;
    }
    return finalEntry->getFrequency();
}


float ForwardingInformationBase::getLPMRtt(const Name& name)
{
    int maxScore = 0;   
    FIBEntry* finalEntry = NULL;
    for(auto& list : entries)
	{
		for(unsigned int i = 0; i < list.second.size(); i++)
		{
			FIBEntry* entry = list.second[i];
            int curScore = computeLongestCommonPrefixSize(name, entry->getName()); 
            if(curScore > maxScore) 
            {
                maxScore = curScore;
                finalEntry = entry;
            }
        }
    }

    if(finalEntry == NULL)
    {
        return INT_MAX;
    }
    return finalEntry->getRtt();   
}

/*************************
 *    private functions  *
 *************************/
unordered_map<Interface*, int> ForwardingInformationBase::initScoreMap(void)
{
	unordered_map<Interface*, int> scoreMap;
	for(Interface* interface : this->interfaces)
	{
		scoreMap[interface] = 0;
	}
	return scoreMap;
}

void ForwardingInformationBase::computeScores(unordered_map<Interface*, int>& scoreMap, const Name& name)
{
	for(auto& list : entries)
	{
		for(unsigned int i = 0; i < list.second.size(); i++)
		{
			FIBEntry* entry = list.second[i];
			int curScore = computeLongestCommonPrefixSize(name, entry->getName());
			if(curScore > scoreMap[entry->getInterface()])
			{
				scoreMap[entry->getInterface()] = curScore;
			}
		}
	}
}

bool ForwardingInformationBase::isFirstForward(unordered_map<Interface*, int>& scoreMap)
{
	int totalScore = 0;
	for(auto& item: scoreMap)
	{
		totalScore += item.second;
	}

	return totalScore <= 0;
}

vector<Interface*> ForwardingInformationBase::sortInterfaces(unordered_map<Interface*, int>& scoreMap)
{
	/*now sort the interfaces according to the score (descendingly)*/
	vector<Interface*> intarr;
	for(auto pair : scoreMap)
	{
		intarr.push_back(pair.first);
	}

	for(unsigned int i = 0; i < scoreMap.size(); i++)
	{
		int maxScore = 0;
		Interface* maxInterface = NULL;
		int pos = i;
		for(unsigned int j = i; j < scoreMap.size(); j++)
		{
			if(scoreMap[intarr[j]] >= maxScore)
			{
				maxScore = scoreMap[intarr[j]];
				maxInterface = intarr[j];
				pos = j;
			}
		}
		
		intarr[pos] = intarr[i];
		intarr[i] = maxInterface;
	}
	return intarr;	
}

vector<Interface*> ForwardingInformationBase::calculateFinalSetAccordingToPriority(vector<Interface*>& intarr, uint8_t priority)
{
	vector<Interface*> interfaces;
	if(intarr.size() <= priority)
	{
		interfaces.push_back(intarr[0]); /*if there is not enough interfaces to remove just send to the first one*/
	}
	else
	{
		/*forward to as many interfaces as the priority allows*/
		int n_removed = intarr.size() - priority;
		for(int i = 0; i < n_removed; i++)
		{
			interfaces.push_back(intarr[i]);
		}
	}

	return interfaces;
}


/***********************
 *    Static helper    *
 ***********************/ 


/**
  * EFFECTS: calculates how many prefix consecutive components of the given two names are equal
  * RETUNRS: the number of matching components
  */
static int computeLongestCommonPrefixSize(const Name& name1, const Name& name2)
{
	size_t n = min(name1.size(), name2.size());
	int score = 0;

	for(size_t i = 0; i < n; i++)
	{
		name::Component comp1 = name1.at(i);
		name::Component comp2 = name2.at(i);
		if(comp1.compare(comp2) == 0)
		{
			++score;
		}
		else
		{
			break;
		}
	}
	return score;	
}


