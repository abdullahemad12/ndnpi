#include <data/Graph.hpp>
#include <vector>
#include<bits/stdc++.h> 

using namespace std;

Graph::Graph(int** edges, int n, int m, int sourceNode)
{
    this->sourceNode = sourceNode;

    constructGraph(edges, n, m);
    
    sortAndCopy(edges,  m);
}




/******************
 * private helpers*
 ******************/

void Graph::constructGraph(int** edges, int n, int m)
{
    for(int i = 0; i < n; i++)
    {
        vector<Pair> vec;
        graph.push_back(vec);
    }

    for(int i = 0; i < m; i++)
    {
        int v1 = edges[i][0];
        int v2 = edges[i][1];
        int cost = edges[i][2];
        Pair p1(v2, cost);  
        Pair p2(v1, cost);

        graph[v1].push_back(p1);
        graph[v2].push_back(p2);
    }
}
void Graph::sortAndCopy(int** edges, int m)
{
    /*store the edges (sorted by score) to make the calculation of the MST easier*/
    bool visited[m];
    for(int i = 0; i < m; i++)
    {
        vector<int> vec;
        int min = INT_MAX;
        int minIndex = 0;
        for(int j = 0; j < m; j++)
        {
            if(!visited[j] && edges[j][2] < min)
            {
                minIndex = j;
                min = edges[j][2];
            }
        }
        vec.push_back(edges[minIndex][0]);
        vec.push_back(edges[minIndex][1]);
        vec.push_back(edges[minIndex][2]);
        visited[minIndex] = true;
        this->edges.push_back(vec);
    }

}
