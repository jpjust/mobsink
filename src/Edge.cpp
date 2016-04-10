#include <stdlib.h>
#include "Edge.h"

// Default constructor
Edge::Edge(Vertex *src, Vertex *dst) throw(edge_exception)
{
    try
    {
        SetSource(src);
        SetDestination(dst);
    }
    catch(edge_exception e)
    {
        throw;
    }
}

// Operator ==
bool Edge::operator==(Edge e)
{
    return ((*src == *e.GetSource()) && (*dst == *e.GetDestination())) || ((*dst == *e.GetSource()) && (*src == *e.GetDestination()));
}

// Getters and setters
Vertex *Edge::GetSource(void)
{
    return this->src;
}

Vertex *Edge::GetDestination(void)
{
    return this->dst;
}

void Edge::SetSource(Vertex *src) throw(edge_exception)
{
    if (src != NULL)
        this->src = src;
    else
        throw EX_EDGE_INVALID_SRC;
}

void Edge::SetDestination(Vertex *dst) throw(edge_exception)
{
    if (dst != NULL)
        this->dst = dst;
    else
        throw EX_EDGE_INVALID_DST;
}
