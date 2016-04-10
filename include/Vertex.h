#ifndef VERTEX_H
#define VERTEX_H

#include "Point.h"

// This class represents a vertex in a graph
class Vertex
{
public:
    Vertex(void);
    Vertex(Point p);
    bool operator==(Vertex v);
    bool operator!=(Vertex v);

    Point GetPoint(void);
    void SetPoint(Point p);

    // Dijkstra's algorithm specific methods
    void Dijkstra_Initialize(void);
    float Dijkstra_GetDist(void);
    bool Dijkstra_GetVisited(void);
    Vertex *Dijkstra_GetBefore(void);
    void Dijkstra_SetDist(float dist);
    void Dijkstra_SetVisited(bool visited);
    void Dijkstra_SetBefore(Vertex *before);

private:
    Point p;

    // Dijkstra's algorithm specific attributes
    float dij_dist;
    bool dij_visited;
    Vertex *dij_before;
};

#endif // VERTEX_H
