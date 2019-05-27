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

using namespace std;
/**
  * Prototypes for helper functions
  */
static int computeLongestCommonPrefixSize(const Name& name1, const Name& name2);
static char* readline(FILE* file);
static int str_split(char* str, char c, char*** strret);
static void destroy_char_arr(char** arr, int length);

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
void ForwardingInformationBase::parseTable(const char* tpath)
{
	FILE* file = fopen(tpath, "r");
	if(file == NULL)
	{
		cout << "Couldn't open the routing table file\n";
		exit(1);
	}
	

	while(1)
	{
		char* line = readline(file);
		if(line == NULL)
		{
			break;
		}
		char** arr;
		int n = str_split(line, ' ', &arr);
		if(n != 3)
		{
			cout << "Invalid Routing Table Format\n";
			exit(1);		
		}
		string ip(arr[1]);
		string port(arr[2]);
		Interface* interface = new Interface(ip, port);
		interfaces.push_back(interface);

		/*free memory to avoid memory leaks*/
		destroy_char_arr(arr, n);
		free(line);
	}

	fclose(file);
}

ForwardingInformationBase::ForwardingInformationBase(const char* tpath)
{
	this->parseTable(tpath);
}

ForwardingInformationBase::~ForwardingInformationBase(void)
{
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


	
	vector<Interface*> intarr = sortInterfaces(scoreMap);
	
	interfaces = calculateFinalSetAccordingToPriority(intarr, interest.getPriority());
	
	return interfaces;
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
		FIBEntry* entry = new FIBEntry(name, interface, rtt);
		entries[key] = entry;
	}
	else
	{
		entries[key]->setRtt(rtt);
	}
	entriesLock.unlock();
}

void ForwardingInformationBase::remove(Request& request)
{
	string key = request.getInterestNameUri();

	entriesLock.lock();
	if(entries.find(key) != entries.end())
	{
		entries.erase(key);
	}
	entriesLock.unlock();
}

vector<Interface*> ForwardingInformationBase::getInterfaces(void)
{
	return interfaces;
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
	for(auto& item : entries)
	{
		FIBEntry* entry = item.second;
		int curScore = computeLongestCommonPrefixSize(name, entry->getName());
		if(curScore > scoreMap[entry->getInterface()])
		{
			scoreMap[entry->getInterface()] = curScore;
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



/**
  * EFFECT: reads one line from the given file
  * MODIFIES: file
  * REQUIRES: the file to opened successfully
  * NOTE: the returned string should be freed using free after it's no longer useful
  */
static char* readline(FILE* file)
{
	char* s = (char*) malloc(sizeof(char) * 1000);

	if(fgets(s, 999, file) == NULL)
	{
		free(s);
		return NULL;
	}

	int n = strlen(s);
	char* ret = (char*) malloc(sizeof(char) * (n + 1));
	for(int i = 0; i < n; i++)
	{
		if(s[i] !='\n')
		{
			ret[i] = s[i];
		}
		else
		{
			ret[i] = '\0';
			break;
		}
	}
	ret[n] = '\0';
	free(s);
	return ret;
}

/**
  * EFFECTS: splits the given string with the given dilemeter
  * RETURNS: the size of the array
  * PARAMTERS:
  * - char* str: the string to be 
  * - char dil: the dilemeter
  */
static int str_split(char* str, char c, char*** strret)
{
	int m = 1; /*the number of words*/
	int n = strlen(str);
	for(int i = 0; i < n; i++)
	{
		if(i < n - 1 && str[i] == c)
		{
			++m;
		}
	}
	char** arr = (char**)malloc(sizeof(char*) * m);


	int ptr = 0;	
	
	for(int i = 0; i < m; i++)
	{
		int j;
		for(j = ptr; j < n && str[j] != c; j++);
		int len = (j - ptr) + 1;
		char* word = (char*)malloc(sizeof(char) * len);
		int tmpptr = 0;
		for(int k = ptr; k < j; k++)
		{
			word[tmpptr++] = str[k];
		}
		word[tmpptr] = '\0'; 
		arr[i] = word;
		ptr = j + 1; 
	}
	*strret = arr;
	return m;
}

static void destroy_char_arr(char** arr, int length)
{
	for(int i = 0; i < length; i++)
	{
		free(arr[i]);
	}
	free(arr);
}
