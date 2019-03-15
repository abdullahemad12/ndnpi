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
#include <stdlib.h>
#include <string.h>
#include <datastructures/hashtable.h>
#include <datastructures/linkedlist.h>

void (*cur_free_ptr)(void*);

static void destroy_entry(void* entry_obj)
{	
	ht_entry_t* entry = (ht_entry_t*) entry_obj;

	cur_free_ptr(entry->value);
	free(entry->key);
	free(entry);
}


static long ht_hash(char* key)
{
	const int p = 53;
	const int m = 1e9 + 9;
	long hashvalue = 0;
	long p_pow = 1;	

	for(int i = 0, n = strlen(key); i < n; i++)
	{
		hashvalue += (key[i] * p_pow) % m;
		p_pow = p_pow * p;
	}
	
	return hashvalue;
}

static bool ht_compare(void* curObj, void* targetObj)
{
	ht_entry_t* cur = (ht_entry_t*) curObj;
	char* target = (char*) targetObj;

	if(strcmp(cur->key, target) == 0)
	{
		return true;
	}
	return false;
} 
hashtable_t* ht_create(unsigned int n)
{
	hashtable_t* ht = malloc(sizeof(hashtable_t) + ((sizeof(linkedlist_t*) + 1) * n));
	if(ht == NULL)
	{
		return NULL;
	}

	ht->n = n;
	ht->size = 0;
	return ht;
}

void ht_put(hashtable_t* ht, char* key, void* value)
{

	long hash = ht_hash(key);
	int index = hash % ht->n;
	ht_entry_t* entry = ll_search(ht->buckets[index], key, ht_compare);
	if(entry == NULL)
	{
		entry = malloc(sizeof(ht_entry_t));
		if(entry == NULL)
		{
			return;
		}
		entry->key = malloc(sizeof(char) * (strlen(key) + 1));
		if(entry->key == NULL)
		{
			free(entry);
			return;
		}
		strcpy(entry->key, key);	
		entry->value = value;
		ll_add(ht->buckets[index], entry);
		++ht->size;
	}
	else
	{
		entry->value = value;
	}

}

void* ht_get(hashtable_t* ht, char* key)
{
	long hash = ht_hash(key);
	int index = hash % ht->n;
	ht_entry_t* entry = ll_search(ht->buckets[index], key, ht_compare);

	return entry;
}

void* ht_remove(hashtable_t* ht, char* key)
{
	long hash = ht_hash(key);
	int index = hash % ht->n;
	ht_entry_t* entry = ll_search(ht->buckets[index], key, ht_compare);
	if(entry == NULL)
	{
		return NULL;
	}
	ll_remove(ht->buckets[index], entry);
	
	void* value = entry->value;
	
	free(entry->key);
	free(entry);
	--ht->size;
	return value;
}

bool ht_contains_key(hashtable_t*ht, char* key)
{
	long hash = ht_hash(key);
	int index = hash % ht->n;
	ht_entry_t* entry = ll_search(ht->buckets[index], key, ht_compare);

	return entry != NULL;
}

unsigned int ht_size(hashtable_t* ht)
{
	return ht->size;
}

void ht_destroy(hashtable_t* ht, void (*free_ptr)(void*))
{
	cur_free_ptr = free_ptr;
	
	for(int i = 0; i < ht->n; i++)
	{
		if(ht->buckets[i] != NULL)
		{
			ll_destroy(ht->buckets[i], destroy_entry);
		}
	}

	free(ht);
}