/*
 * Cluster class for MobSink.
 * Copyright (C) 2015-2018 João Paulo Just Peixoto <just1982@gmail.com>.
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
    double GetDrops(void);
    float GetMovedPixels(void);
    vector<Point> GetLastPath(void);
    void SetLastPath(vector<Point> last_path);
    void IncrementDist(float dist);

    void IncreasePDUs(void);
    void IncreaseDrops(double packets);
    void ResetPDUs(void);
    void ResetTravels(void);

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
    void SetCurrentTime(unsigned int t);
    void StopSinkUntil(unsigned int t);
    bool SinkCanReceive(void);
    void AddTravel(unsigned int t);
    double GetAvgTravelTime(void);
    unsigned int GetLastTravelTime(void);

private:
    Sink sink;
    Point mean;
    vector<Node *> nodes;
    int RL;
    double received_pdus;
    double dropped_pdus;
    float moved_pixels;
    vector<Point> last_path;
    unsigned int cur_time = 0;
    unsigned int stopped_until = 0;
    vector<unsigned int> travels;
};

#endif // CLUSTER_H
