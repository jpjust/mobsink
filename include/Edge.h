/*
 * Graph's Edge class for MobSink.
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

#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"
#include "Path.h"

// Edge exceptions
enum edge_exception {
    EX_EDGE_INVALID_SRC,
    EX_EDGE_INVALID_DST,
};

class Vertex;

// This class represents an edge in a graph
class Edge
{
public:
    Edge(Vertex *src, Vertex *dst, map<int, struct path_control_params> *path_control, float speedlimit = 0);
    bool operator==(Edge e);

    Vertex *GetSource(void);
    Vertex *GetDestination(void);
    void SetSource(Vertex *src);
    void SetDestination(Vertex *dst);
    void SetPathControl(map<int, struct path_control_params> *path_control);
    map<int, struct path_control_params> *GetPathControl(void);

    float GetWeight(int time = 0, bool use_traffic = true);
    unsigned int GetTime(int time = 0);
    float GetLength(void);
    float GetSpeedLimit(void);

private:
    Vertex *src;
    Vertex *dst;
    map<int, struct path_control_params> *path_control;
    float speedlimit;
};

#endif // EDGE_H
