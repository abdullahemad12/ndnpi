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


#ifndef _DATA_GRAPH_
#define _DATA_GRAPH_

#include <vector>

using namespace std;


class Pair
{
    public:
        int x;
        int y;
    Pair(int x, int y)
    {
        this->x = x; 
        this->y = y;
    }
};

class Graph
{
    private:
        vector<vector<Pair>> graph;
        vector<vector<int>> edges;
        int sourceNode; 
        
        /**
          * EFFECTS: makes a sorted copy of edges and stores them in this->edges
          * MODIFIES: this
          */
       void sortAndCopy(int** edges, int m);
        /**
          * EFFECTS: constructs the graph from the edges
          * MODIFIES: this
          * PARAMETERS:
          * int** edges: the edges in the graph
          * int n: the number of nodes in the graph
          * int m: the number of edges in the graph
          */
       void constructGraph(int** edges, int n, int m);


    public: 
        Graph(int** edges, int n, int m, int sourceNode);
    
};

#endif 
