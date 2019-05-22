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

/**
  * Prototypes for helper functions
  */
static bool compare(FIBEntry* entry, Name* name);
static int computeLongestCommonPrefixSize(Name* name1, Name* name2);
static char* readline(FILE* file);
static int str_split(char* str, char c, char*** strret);
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
		ll_add(this->faces, interface);
	}

	fclose(file);
}

ForwardingInformationBase::ForwardingInformationBase(const char* tpath)
{
	this->faces = ll_create();
	if(this->faces == NULL)
	{
		std::cout << "ENOMEM\n";
		exit(1);
	}
	this->entries = ll_create();
	if(this->entries == NULL)
	{
		std::cout << "ENOMEM\n";
		exit(1);
	}
	this->parseTable(tpath);
}


vector<Interface*> ForwardingInformationBase::computeMatchingFaces(Name* name)
{

	vector<Interface*> faces;
	
	int maxScore = 0;
	Interface* ret = NULL;

	/**
      * Iterate over the linkedlist
      */
	struct ll_node* cur = this->entries->head;
	while(cur != NULL)
	{
		FIBEntry* entry = (FIBEntry*) cur->object;
		int curScore = computeLongestCommonPrefixSize(entry->getName(), name);
		if(curScore > maxScore)
		{
			ret = entry->getInterface();
			maxScore = curScore;
		}
		cur = cur->next;
	}
	ret = NULL; /*for now forward to all faces*/
	if(ret == NULL)
	{
		cur = this->faces->head;
		while(cur != NULL)
		{
			faces.push_back((Interface*) cur->object);
			cur = cur->next;	
		}
	}
	else
	{
		faces.push_back(ret);
	}
	return faces;	
}

struct linkedlist* ForwardingInformationBase::getFaces(void)
{
	return this->faces;
}


void ForwardingInformationBase::insert(Name* name, Interface* interface)
{
	FIBEntry* entry = (FIBEntry*) ll_search(this->entries, name, (bool (*)(void*, void*)) compare);
	if(entry != NULL)
	{
		return;
	}

	entry = new FIBEntry(name, interface);

	ll_add(this->entries, entry);
}

/***********************
 *    Static helper    *
 ***********************/ 
/**
  * EFFECTS: used to search for an entry in the linkedlist
  * RETURNS: true if the name of the entry and the given name match
  */
static bool compare(FIBEntry* entry, Name* name)
{
	return entry->getName()->compare(*name) == 0;
}

/**
  * EFFECTS: calculates how many prefix consecutive components of the given two names are equal
  * RETUNRS: the number of matching components
  */
static int computeLongestCommonPrefixSize(Name* name1, Name* name2)
{
	size_t n = min(name1->size(), name2->size());
	int score = 0;

	for(size_t i = 0; i < n; i++)
	{
		name::Component comp1 = name1->at(i);
		name::Component comp2 = name2->at(i);
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
