#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Edge.h"
#include "Vertex.h"
#include "Point.h"

using namespace std;

// This class represents a graph
class Graph
{
public:
    Graph();

    vector<Vertex *> GetVertices(void);
    vector<Edge *> GetEdges(void);

    int FindVertex(Vertex v);
    bool HasVertex(Vertex v);
    Vertex *InsertVertex(Point p);
    Vertex *InsertVertexAndConnect(Point p);
    bool DeleteVertex(Vertex v);
    int FindEdge(Edge e);
    bool HasEdge(Edge e);
    Edge *InsertEdge(Vertex *src, Vertex *dst, int weight = 1);
    bool DeleteEdge(Edge e);

    vector<Point> Dijkstra(Vertex *src, Vertex *dst);

    void Clear(void);

private:
    vector<Vertex *> vertices;
    vector<Edge *> edges;
};

#endif // GRAPH_H
