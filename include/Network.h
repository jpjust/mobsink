/*
 * MobSink Network modeling.
 * Copyright (C) 2015-2019 João Paulo Just Peixoto <just1982@gmail.com>.
 *
 * This file is part of MobSink.
 *
 * MobSink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MobSink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MobSink.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        bool PositionSinks(bool use_runtime_RL, int sinkpos, int t = 0, bool use_traffic = true);
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
        bool graph_ready;
};

#endif // NETWORK_H
