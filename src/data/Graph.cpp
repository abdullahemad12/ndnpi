#include <data/Graph.hpp>
#include <vector>

using namespace std;

Graph::Graph(int** edges, int n, int m, int sourceNode)
{
    this->sourceNode = sourceNode;

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

