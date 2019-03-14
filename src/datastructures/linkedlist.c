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
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <datastructures/linkedlist.h>

/**
  * Prototypes
  */
static ll_node_t* get_node_at(linkedlist_t* ll, unsigned int i);

linkedlist_t* ll_create(void)
{
	linkedlist_t* ll = malloc(sizeof(linkedlist_t));
	if(ll == NULL)
	{
		return NULL;
	}

	ll->head = NULL;	
	ll->size = 0;
	return ll;
}


void ll_add(linkedlist_t* ll, void* object)
{
	if(ll->head == NULL)
	{
		ll->head = malloc(sizeof(ll_node_t));
		if(ll->head == NULL)
		{
			return;
		}
		ll->head->object = object;
		ll->head->next = NULL;
		ll->head->prev = NULL;
	}
	else
	{
		ll_node_t* node = malloc(sizeof(ll_node_t));
		if(node == NULL)
		{
			return;
		}
		node->next = ll->head;
		node->prev = NULL;
		ll->head->prev = node;
		ll->head = node;
		node->object = object;
	}
	++ll->size;
}

void* ll_get_at(linkedlist_t* ll, unsigned int i)
{
	if(i >= ll->size)
	{
		return NULL;
	}
	
	ll_node_t* ret = get_node_at(ll, i);
	return ret->object;
}

void* ll_remove_at(linkedlist_t* ll, unsigned int i)
{
	if(i >= ll->size)
	{
		return NULL;
	}
	ll_node_t* ret = get_node_at(ll, i);
	void* obj = NULL;
	if(ret == ll->head)
	{
		if(ret->next != NULL)
		{
			ret->next->prev = NULL;
		}
		ll->head = ret->next;
		obj = ret->object;
		free(ret);
	}
	else
	{
		ret->prev->next = ret->next;
		obj = ret->object;
		free(ret);
	}
	--ll->size;
	return obj;
}

void* ll_remove(linkedlist_t* ll, void* object)
{
	ll_node_t* ret = ll->head;
		
	while(ret != NULL  && ret->object != object)
	{
		ret = ret->next;
	}

	void* obj = NULL;
	if(ret == NULL)
	{
		return NULL;
	}
	else if(ret == ll->head)
	{
		if(ret->next != NULL)
		{
			ret->next->prev = NULL;
		}
		ll->head = ret->next;
		obj = ret->object;
		free(ret);
	}
	else
	{
		ret->prev->next = ret->next;
		obj = ret->object;
		free(ret);
	}
	return obj;
}

void* ll_search(linkedlist_t* ll, void* object, bool (*compare) (void*, void*))
{
	ll_node_t* node = ll->head;
	while(node != NULL)
	{
		if(compare(node->object, object))
		{
			return node->object;
		}
	}
	return NULL;
}

void ll_destroy(linkedlist_t* ll, void (*free_ptr)(void*))
{
	ll_node_t* cur = ll->head;
	ll_node_t* del = NULL;
	while(cur != NULL)
	{
		del = cur;
		cur = cur->next;
		if(free_ptr != NULL)
		{
			free_ptr(del->object);
		}
		free(del);
	}
	free(ll);
}


/*******************
 *                 *
 *     Helpers     *  
 *                 *
 *******************/


/** 
  * EFFECTS: gets the node at index i
  * REQUIRES: i < ll->size
  * RETURNS: pointer to the target node
  */
static ll_node_t* get_node_at(linkedlist_t* ll, unsigned int i)
{
	ll_node_t* ret = ll->head;
	for(int j = 0; j < i; j++){
		ret = ret->next;
	}
	return ret;
}
 