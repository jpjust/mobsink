#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"

// Edge exceptions
enum edge_exception {
    EX_EDGE_INVALID_SRC,
    EX_EDGE_INVALID_DST,
};

// This class represents an edge in a graph
class Edge
{
public:
    Edge(Vertex *src, Vertex *dst) throw(edge_exception);
    bool operator==(Edge e);

    Vertex *GetSource(void);
    Vertex *GetDestination(void);
    void SetSource(Vertex *src) throw(edge_exception);
    void SetDestination(Vertex *dst) throw(edge_exception);

private:
    Vertex *src;
    Vertex *dst;
};

#endif // EDGE_H
