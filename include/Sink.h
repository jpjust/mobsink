#ifndef SINK_H
#define SINK_H

#include "Node.h"
#include <stdlib.h>

#define SINK_SPEED  2

class Sink: public Node
{
public:
    Sink(float x = 0, float y = 0, int RL = 0, Cluster *cluster = NULL, Node *nexthop = NULL);
    bool GetConnected(void);
    void SetConnected(bool connected);
    double GetData(void);
    float Walk(Point dest);

private:
    bool connected;
};

#endif // SINK_H
