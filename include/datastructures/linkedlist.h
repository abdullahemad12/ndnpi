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

#ifndef _DATASTRUCTURES_LINKEDLIST_H_
#define _DATASTRUCTURES_LINKEDLIST_H_


typedef struct ll_node
{
	void* object;
	struct ll_node* next;
	struct ll_node* prev; 
} ll_node_t;


typedef struct linkedlist
{
	ll_node_t* head;
	ll_node_t* tail;
	unsigned int size;
} linkedlist_t;


/**
  * void -> linkedlist_t*
  * EFFECTS: creates a linkedlist object
  * RETURNS: pointer to the linkedlist object
  */
linkedlist_t* ll_create(void);

/**
  * linkedlist_t*, void* -> void
  * EFFECTS: adds a new object to the linkedlist
  * MODIFIES: ll
  * REQUIRES: the object to point to a valid memory address
  * PARAMETERS:
  * - linkedlist_t* ll: the linkedlist to be modified
  * - void* object: pointer to the object to be added to the linkedlist
  */
void ll_add(linkedlist_t* ll, void* object);

/**
  * linkedlist_t*, unsigned int -> void*
  * EFFECTS: gets a the object at a certain object indexed by i from the linkedlist
  * REQUIRES: i to be less than the linkedlist's size by at least 1 and to be nonnegative
  * RETURNS: the deleted object
  * PARAMETERS: 
  * - linkedlist_t* ll: the linkedlist
  * - unsigned int i: the index of the object in the linkedlist
  */
void* ll_get_at(linkedlist_t* ll, unsigned int i);

/**
  * linkedlist_t*, unsigned int -> void*
  * EFFECTS: remove the object at the index i
  * REQUIRES:  0 =< i < ll->size
  * MODIFIES: ll
  * RETURNS: the deleted object
  * PARAMETERS: 
  * - linkedlist_t* ll: the linkedlist
  * - unsigned int i: the index of the object in the linkedlist
  */
void* ll_remove_at(linkedlist_t* ll, unsigned int i);

/**
  * linkedlist*, void* -> void*
  * EFFECTS: removes the given object from the linkedlist if it exists
  * MODIFIES: ll
  * RETURNS: the deleted object or NULL if the object does not exist
  * PARAMETERS:
  * - linkedlist_t* ll: the linkedlist
  * - void* object: the object to be removed
  */
void* ll_remove(linkedlist_t* ll, void* object);

/**
  * linkedlist_t* -> void
  * EFFECTS: destroys the given linkedlist object
  * MODIFIES: ll 
  * REQUIRES: the given ll to be created by ll_create
  * PARAMETERS:
  * -linkedlist_t* ll: the linkedlist
  */ 
void ll_destroy(linkedlist_t* ll);

#endif /*..._DATASTRUCTURES_LINKEDLIST_H_*/