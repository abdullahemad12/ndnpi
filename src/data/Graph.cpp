#include <data/Graph.hpp>
#include <vector>
#include<bits/stdc++.h> 
#include <datastructures/UnionFind.hpp>

using namespace std;

Graph::Graph(int** edges, int n, int m, int sourceNode)
{
    this->sourceNode = sourceNode;

    constructGraph(edges, n, m);
    
    sortAndCopy(edges,  m);
}


void Graph::calculateMST(void)
{
    int n = graph.size();

    UnionFind uf(n);
    for(int i = 0; i < n; i++)
    {
        vector<Pair> vec;
        tree.push_back(vec);
    }
    for(vector<int> edge : edges)
    {
        if(!uf.isInSameComponent(edge[0], edge[1]))
        {
           addEdgeToTree(edge[0], edge[1], edge[2]);
           uf.join(edge[0], edge[1]);
        }

    }
  
}

vector<int> Graph::getSourceNeighbors(void)
{
    vector<Pair> edges = graph[sourceNode];

    vector<int> incident;
    for(Pair pair : edges)
    {
        incident.push_back(pair.vertex);
    }

    return incident;
}

int Graph::calculateNextHop(int destination)
{
    
    for(unsigned int i = 0; i < tree[sourceNode].size(); i++)
    {   
        int vertex = tree[sourceNode][i].vertex;
        bool* visited = new bool[tree.size()];
        for(unsigned int i = 0; i < tree.size(); i++) visited[i] = false;
        visited[sourceNode] = true;
        visited[vertex] = true;
        bool reachable = canBeReached(vertex, destination, visited);
        delete visited;
        if(reachable)
        {
            return vertex;
        }
    }



    return -1;
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
        addEdge(edges[i][0], edges[i][1], edges[i][2]);
    }
}
void Graph::sortAndCopy(int** edges, int m)
{
    /*store the edges (sorted by score) to make the calculation of the MST easier*/
    bool visited[m];
    for(int i = 0; i < m; i++)
    {
        visited[i] = false;
    }
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

void Graph::addEdge(int v1, int v2, int cost)
{
    Pair p1(v2, cost);  
    Pair p2(v1, cost);

    graph[v1].push_back(p1);
    graph[v2].push_back(p2);
}

void Graph::addEdgeToTree(int v1, int v2, int cost)
{
    Pair p1(v2, cost);  
    Pair p2(v1, cost);

    tree[v1].push_back(p1);
    tree[v2].push_back(p2);
}
bool Graph::canBeReached(int source, int destination, bool* visited)
{
    if(source == destination)
    {
        return true;
    }
    bool ret = false;
    for(unsigned int i = 0; i < tree[source].size(); i++)
    {
        int vertex = tree[source][i].vertex;
        if(!visited[vertex])
        {   
            visited[vertex] = true;
            ret = ret || canBeReached(vertex, destination, visited);
        }
    }

    return ret;
}
