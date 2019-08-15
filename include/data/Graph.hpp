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
        int vertex;
        int cost;
    Pair(int vertex, int cost)
    {
        this->vertex = vertex; 
        this->cost = cost;
    }
};

class Graph
{
    private:
        vector<vector<Pair>> graph;
        vector<vector<Pair>> tree;
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

        /**
          * EFFECTS: adds a new undirected edge to the graph
          * MODIFIES: this 
          * PARAMETERS: 
          * int v1: vertex1
          * int v2: vertex2
          * int cost: the cost of the edge
          */   
        void addEdge(int v1, int v2, int cost);
        void addEdgeToTree(int v1, int v2, int cost);

        /**
          * EFFECTS: checks if the destination node can be reached from the source
          * RETURNS: true if the source can be reached, false otherwise
          */
       bool canBeReached(int source, int destination, bool* visited);

    public: 
        Graph(int** edges, int n, int m, int sourceNode);

        /**
          * EFFECTS: calculates the minimum spanning tree on the current graph
          * MODIFIES: this
          * REQUIRES: the edges to be sorted. Otherwise the spanning tree created is not minimum
          */
        void calculateMST(void);
    
        /**
          * EFFECTS: gets the id of the verticies directly connected to the source vertex
          */
        vector<int> getSourceNeighbors(void);
  

        /**
          * EFFECTS: calculates the nexthop in the graph according to the MST
          * MODIFIES: this
          * REQUIRES: calculateMST to be called one time before this is called
          */
        int calculateNextHop(int destination);

        /**
          * EFFECTS: calculates the cost of reaching some destination node 
          * MODIFIES: this
          */
        int calculateCost(int destination);

        vector<int> calculateNextHops(int destination);
};

#endif 
