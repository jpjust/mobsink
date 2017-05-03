/*
 * Graph's Edge class for MobSink.
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
    Edge(Vertex *src, Vertex *dst, map<int, struct path_control_params> *path_control = NULL) throw(edge_exception);
    bool operator==(Edge e);

    Vertex *GetSource(void);
    Vertex *GetDestination(void);
    void SetSource(Vertex *src) throw(edge_exception);
    void SetDestination(Vertex *dst) throw(edge_exception);
    void SetPathControl(map<int, struct path_control_params> *path_control);
    map<int, struct path_control_params> *GetPathControl(void);

    int GetLenght(int time = 0);
private:
    Vertex *src;
    Vertex *dst;
    map<int, struct path_control_params> *path_control;
};

#endif // EDGE_H
