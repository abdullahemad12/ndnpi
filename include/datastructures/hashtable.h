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

#ifndef _DATASTRUCTURES_HASHTABLE_H_
#define _DATASTRUCTURES_HASHTABLE_H_

#include <datastructures/linkedlist.h>

typedef struct ht_entry
{
	char* key;
	void* value;
} ht_entry_t;


typedef struct hashtable
{
	unsigned int size;
	unsigned int n; 
	linkedlist_t** buckets;
} hashtable_t;


/**
  * unsigned int -> hashtable_t
  * EFFECTS: creates a hashtable with n buckets
  * RETUNRS: a pointer to the new hashtable
  * PARAMETERS:
  * - unsigned int n: the number of buckets, the hashtable will have
  */
hashtable_t* ht_create(unsigned int n);

/**
  * hashtable_t*, char*, void* -> void
  * EFFECTS: inserts a new element in the hashtable
  * 		 if the key already exist, replaces the current value
  * MODIFIES: ht
  * PARAMETERS:
  * - hashtable_t* ht: the hashtable to be modified
  * - char* key: the key that will be used to hash the given object
  * - void* value: the value that will be stored in the hashtable
  */
void ht_put(hashtable_t* ht, char* key, void* value);


/** 
  * hashtable_t*, char* -> void*
  * EFFECTS: given the key, gets the value from the hashtable
  * RETURNS: the object, Or NULL if no such key was found 
  * PARAMETERS:
  * - hashtable_t* ht: the hastable
  * - char* key: the key that will be used to retrieve the object
  */
void* ht_get(hashtable_t* ht, char* key);


/**
  * hashtable_t*, char* -> void*
  * EFFECTS: given the key, removes the value from the hashtable 
  * MODIFIES: ht
  * RETURNS: the object that was removed, or NULL if the key was not found 
  * PARAMETERS:
  * - hashtable_t* ht: the hashtable
  * - char* key: the key of the object to be removed
  */
void* ht_remove(hashtable_t* ht, char* key);

/**
  * hashtable_t*, char* -> bool
  * EFFECTS: checks if the given key is present in the database
  * RETURNS: true if the key was found, false otherwise
  * PARAMETERS:
  * - hashtable_t* ht: the hashtable
  * - char* key: the key to be checked
  */
bool ht_contains_key(hashtable_t*ht, char* key);

/**
  * hashtable_t* -> unsigned int 
  * EFFECTS: gets the size of the hashtable
  * RETURNS: the size as an int
  */
unsigned int ht_size(hashtable_t* ht);

/**
  * hashtable_t*, void (*)(void*) -> void
  * EFFECTS: frees all the memory associated with the given hashtable
  * MODIFIES: ht
  * REQUIRES: the given function to take a void* as a parameter (the value that was stored in the database)
  *           and returns nothing
  * PARAMETERS:
  * - hashtable_t* ht: the hashtable to be destroyed 
  * - void (*free_ptr)(void*)
  */
void ht_destroy(hashtable_t* ht, void (*free_ptr)(void*));

#endif /*...._DATASTRUCTURES_HASHTABLE_H_*/