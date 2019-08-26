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
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <ndn-cxx/face.hpp>
#include <datastructures/linkedlist.h>
#include <vector>
#include <iostream>
#include <data/FIBEntry.hpp> 
#include <data/Request.hpp>
#include <unordered_map>
#include <fstream>
#include <float.h>
#include <ndnpi.hpp>
#include <MDP/MDP.hpp>

using namespace std;
using namespace boost::numeric;

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
    
    vector<Interface> interfaces;
    for(int i = 0; i < nInterfaces; i++)
    {
        string ip;
        string port;
        file >> ip >> port;
        Interface interface(i, ip, port);
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

    this->graph = new Graph(graph_edges, interfaces.size(), nEdges, source_node);
    this->graph->calculateMST();


    for(int i = 0; i < nEdges; i++)
    {
        delete graph_edges[i];
    }
    delete graph_edges;

    vector<int> incident = graph->getSourceNeighbors();
    
    for(int id : incident)
    {
        Interface* interface = new Interface(interfaces[id].getId(), interfaces[id].getIp(), interfaces[id].getPort());
        interface->connectToFace();
        this->interfaces.push_back(interface); 
    }

    int nPrefixes;

    file >> nPrefixes;

    string* prefixes = new string[nPrefixes];
    int* nodeIds = new int[nPrefixes];
    
    for(int i = 0; i < nPrefixes; i++)
    {
        file >> nodeIds[i] >> prefixes[i];
    }

    for(int i = 0; i < nPrefixes; i++)
    {

        prefixesNodeIds[prefixes[i]] = nodeIds[i];
        Name name(prefixes[i]);
        Interface* interface = NULL;

        int nextHop = graph->calculateNextHop(nodeIds[i]);
        if(nextHop == -1)
        {
            continue;
        }
        for(Interface* interfaceTmp : this->interfaces)
        {
            if(interfaceTmp->getId() == nextHop){
                interface = interfaceTmp;
                break;
            }
        }

        vector<FIBEntry*> list;	
		FIBEntry* entry = new FIBEntry(name, interface, 0);
		list.push_back(entry);
		entries[prefixes[i]] = list;
    }

    

    delete nodeIds;
    file.close();

}
int ForwardingInformationBase::getInterfaceIndex(int id)
{
    int i = 0;
    for(Interface* interface : interfaces)
    {
        if(interface->getId() == id)
        {
            return i;
        }
        ++i;
    }
    return -1;
}
int ForwardingInformationBase::getPrefixId(string name)
{
    int id = 0;
    int maxScore = 0;
    for(auto& entry : prefixesNodeIds)
    {
        int curScore = computeLongestCommonPrefixSize(Name(entry.first), Name(name));
        if(curScore > maxScore) 
        {
            maxScore = curScore;    
            id = entry.second;
        }
    }
    return id;
}

bool ForwardingInformationBase::isReachableThroughNode(string name, int id)
{
    int destination = getPrefixId(name);

    return graph->isReachableThrough(destination, id);
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
    delete graph;
}


void ForwardingInformationBase::updateRewards(void)
{
    vector<float> rtts;
    for(unsigned int i = 0; i < getInterfaces().size(); i++) { rtts.push_back(0); }
    for(auto& list : entries)
	{
		for(unsigned int i = 0; i < list.second.size(); i++)
		{
			FIBEntry* entry = list.second[i];
            Interface* interface = entry->getInterface();
            rtts[getInterfaceIndex(interface->getId())] = entry->getRtt();
        }
    }

   mdp->updateRewards(rtts);

}




vector<Interface*> ForwardingInformationBase::computeMatchingFaces(const Interest& interest)
{
	vector<Interface*> interfaces;
    ublas::matrix<double> policy = mdp->policyIteration();

    int state = shaper->getCurrentState();

    string name = interest.getName().toUri();
    int maxProb = 0;
    int maxI = 0;
    for(unsigned int i = 0; i < this->interfaces.size(); i++)
    {
        int faceId = this->interfaces[i]->getId();
        if(policy(state, i+1) >= maxProb && isReachableThroughNode(name, faceId))
        {
            maxProb = policy(state, i+1);
            maxI = i;
        }
        
    }

    interfaces.push_back(this->interfaces[maxI]);


	return interfaces;
}


void ForwardingInformationBase::updateRTT(const Name& name, float rtt)
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
    assert(finalEntry != NULL);
    finalEntry->setRtt(rtt);
}

void ForwardingInformationBase::insert(Request& request)
{
	
}

void ForwardingInformationBase::remove(string key, Interface* interface)
{
	
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
    int frequency = finalEntry->getFrequency();
    finalEntry->incrementFrequency();
    return frequency;
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

bool ForwardingInformationBase::isNoMatch(unordered_map<Interface*, int>& scoreMap)
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


