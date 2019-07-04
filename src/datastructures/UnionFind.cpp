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

#include <datastructures/UnionFind.hpp>


UnionFind::UnionFind(int size)
{
    parents = new int[size];
    ranks = new int[size];
    this->size = size;
    components = size;
    for(int i = 0; i < size; i++)
    {
        parents[i] = i;
    }
}

UnionFind::~UnionFind(void)
{
    delete parents;
    delete ranks;
}

int UnionFind::find(int i)
{
    int parent = i;

    while(parents[parent] != parent)
    {
        parent = parents[parent];
    }
    parents[i] = parent;

    return parent;
}


bool UnionFind::isInSameComponent(int i, int j)
{
    return find(i) == find(j);
}

void UnionFind::join(int i, int j)
{
    int parentI = find(i);
    int parentJ = find(j);
    if(parentI == parentJ)
    {
        return;
    }
    
    if(ranks[parentI] < ranks[parentJ])
    {
        parents[parentI] = parentJ;
    }
    else if(ranks[parentJ] < ranks[parentI])
    {
        parents[parentJ] = parentI;
    }
    else
    {
        parents[parentJ] = parentI;
        ++ranks[parentI];
    }
    --components;
}
