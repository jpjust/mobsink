/*
 * Graph implementation for MobSink.
 * Copyright (C) 2015-2017 João Paulo Just Peixoto <just1982@gmail.com>.
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

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Edge.h"
#include "Vertex.h"
#include "Point.h"
#include "Path.h"

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
    Edge *InsertEdge(Vertex *src, Vertex *dst, map<int, struct path_control_params> *path_control, float speedlimit = 0);
    bool DeleteEdge(Edge e);

    vector<Point> Dijkstra(Vertex *src, Vertex *dst, int t = 0);

    void Clear(void);

private:
    vector<Vertex *> vertices;
    vector<Edge *> edges;
};

#endif // GRAPH_H
