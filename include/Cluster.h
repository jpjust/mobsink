#ifndef CLUSTER_H
#define CLUSTER_H

#include "Node.h"
#include "Sink.h"
#include "Edge.h"
#include <vector>
#include <stdlib.h>

using namespace std;

class Sink;

class Cluster
{
public:
    Cluster();

    Sink *GetSink(void);
    Point *GetMean(void);
    int GetRL(void);
    double GetPDUs(void);
    float GetMovedPixels(void);
    vector<Point> GetLastPath(void);
    void SetLastPath(vector<Point> last_path);
    void IncrementDist(float dist);

    void IncreasePDUs(void);
    void ResetPDUs(void);

    unsigned int GetNodesCount(void);
    Node *GetNodeAt(unsigned int i);
    void InsertNode(Node *node);
    void RemoveNodeAt(unsigned int i);
    void DeleteNodeAt(unsigned int i);
    void RemoveAll(void);
    void DeleteAll(void);

    void SetMean(float x, float y);
    void MoveSink(float x, float y);
    float MoveSinkDaniel(bool use_runtime_RL);
    float WalkSink(Point dest);
    float UpdateMean(void);
    float UpdateMeanDaniel(void);
    void CleanLinks(void);
    void CreateLinks(float range);
    Node *FindNearestNode(Point p);

private:
    Sink sink;
    Point mean;
    vector<Node *> nodes;
    int RL;
    double received_pdus;
    float moved_pixels;
    vector<Point> last_path;
};

#endif // CLUSTER_H
