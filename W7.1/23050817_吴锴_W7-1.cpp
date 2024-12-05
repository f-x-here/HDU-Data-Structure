#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <cstring>
#include "mystack.h"

using namespace std;
#define INPUTFROMFILE
struct Graph
{
    int adjacency[20][20];
    int inDegree[20];
    int maxn;
    void Initial();
    void FindInDegree();
    void TopologicalSort();
};

void Graph::Initial()
{
    int n, a, b;
    memset(adjacency, 0, sizeof(adjacency));
    maxn = 0;
#ifndef INPUTFROMFILE
    cout << "请输入有向图边个数" << endl;
    cin >> n;
    cout << "请按照 ‘起点 终点’ 的格式输入边" << endl;
    for (int i = 0; i < n; i++)
    {
        cin >> a >> b;
        maxn = max(max(a, b), maxn);
        adjacency[a][b] = 1;
    }
#else
    ifstream in("input.txt");
    in >> n;
    for (int i = 0; i < n; i++)
    {
        in >> a >> b;
        maxn = max(max(a, b), maxn);
        adjacency[a][b] = 1;
    }

#endif
}

void Graph::FindInDegree()
{
    for (int i = 1; i <= maxn; i++)
    {
        inDegree[i] = 0;
        for (int j = 1; j <= maxn; j++)
        {
            inDegree[i] += adjacency[j][i];
        }
    }
}

void Graph::TopologicalSort()
{
    FindInDegree();
    mystack<int> nodes;
    for (int i = 1; i <= maxn; i++)
    {
        if (!inDegree[i])
        {
            nodes.Push(i);
        }
    }
    int count = 0;
    int cur;
    while (!nodes.IsEmpty())
    {
        cur = nodes.Top();
        nodes.Pop();
        cout << " " << cur;
        count++;
        for (int i = 1; i <= maxn; i++)
        {
            if (adjacency[cur][i])
            {
                inDegree[i]--;
                if (!inDegree[i])
                    nodes.Push(i);
            }
        }
    }
    cout << endl;
    if (count < maxn)
    {
        cout << "Error!Cycle Exist!";
    }
    else
    {
        cout << "Finished!";
    }
}

int main()
{
    Graph g;
    g.Initial();
    g.TopologicalSort();
}