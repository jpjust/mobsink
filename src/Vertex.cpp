#include "Vertex.h"
#include <stdlib.h>

// Default constructor
Vertex::Vertex(void)
{
    // Does nothing
}

Vertex::Vertex(Point p)
{
    SetPoint(p);
}

// Operator ==
bool Vertex::operator==(Vertex v)
{
    return GetPoint() == v.GetPoint();
}

// Operator !=
bool Vertex::operator!=(Vertex v)
{
    return GetPoint() != v.GetPoint();
}

// Dijkstra's algorithm initialization
void Vertex::Dijkstra_Initialize(void)
{
    Dijkstra_SetBefore(NULL);
    Dijkstra_SetVisited(false);
    Dijkstra_SetDist((unsigned long int)(-1));
}

// Getters and setters
Point Vertex::GetPoint(void)
{
    return this->p;
}

void Vertex::SetPoint(Point p)
{
    this->p = p;
}

// Dijkstra's algorithm specific getters and setters
float Vertex::Dijkstra_GetDist(void)
{
    return this->dij_dist;
}

bool Vertex::Dijkstra_GetVisited(void)
{
    return this->dij_visited;
}

Vertex *Vertex::Dijkstra_GetBefore(void)
{
    return this->dij_before;
}

void Vertex::Dijkstra_SetDist(float dist)
{
    this->dij_dist = dist;
}

void Vertex::Dijkstra_SetVisited(bool visited)
{
    this->dij_visited = visited;
}

void Vertex::Dijkstra_SetBefore(Vertex *before)
{
    this->dij_before = before;
}
