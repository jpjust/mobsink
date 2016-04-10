#ifndef NETWORK_H
#define NETWORK_H

#include <wx/wx.h>
#include <vector>
#include "Cluster.h"
#include "Node.h"
#include "Obstacle.h"
#include "Path.h"
#include "Graph.h"

using namespace std;

enum
{
    SINKPOS_STATIC,
    SINKPOS_VERTICAL,
    SINKPOS_HORIZONTAL,
    SINKPOS_RLFORGY,
    SINKPOS_RLRANDOM,
    SINKPOS_RLJUST,
};

// This class represents a whole Sensors Network.
// All the nodes will belong to this class. The
// Clusters class is used to keep a reference of
// the nodes that belong to each cluster.
class Network
{
    public:
        Network();
        //void *Entry(void);

        vector<Cluster *> GetClusters(void);
        vector<Node *> GetNodes(void);
        vector<Obstacle> GetObstacles(void);
        vector<Path> GetPaths(void);
        float GetRange(void);
        void SetRange(float range);

        wxSize GetSize(int &leftmost, int &rightmost, int &top, int &bottom);
        static vector<Node *> GridNodes(int n, int RL, int width, int height, int x_start = 0, int y_start = 0);
        void GridSinks(void);
        void CreateClusters(unsigned int n_sinks);
        void ClearClusters(void);
        void Clear(void);
        void InsertNode(Node *n);
        void InsertObstacle(Obstacle o);
        void InsertPath(Path p);
        void InsertSink(Sink *s);
        void UpdateClusters(void);
        void MeansInitializeStatic(void);
        void MeansInitializeForgy(void);
        void MeansInitializeRandom(void);
        void MeansInitializeJust(void);
        void RunKmeans(void);
        bool PositionSinks(bool use_runtime_RL, int sinkpos);
        Cluster *GetNearestClusterFrom(Point p);
        void CreateLinks(void);
        void CheckObstacles(int k);
        void CheckPaths(int k);
        void BuildGraph(void);
        void ClearGraph(void);

    private:
        void BalanceMeans(void);
        vector<Cluster *> clusters;
        vector<Node *> nodes;
        vector<Obstacle> obstacles;
        vector<Path> paths;
        bool static_sinks;
        int sink_move_boundary;
        float txrange;
        Graph G;
};

#endif // NETWORK_H
