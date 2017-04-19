/*
 * Graph's Vertex class for MobSink.
 * Copyright (C) 2015-2016 João Paulo Just Peixoto <just1982@gmail.com>.
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

#ifndef VERTEX_H
#define VERTEX_H

#include "Point.h"

// This class represents a vertex in a graph
class Vertex
{
public:
    Vertex(void);
    Vertex(Point p);
    bool operator==(Vertex v);
    bool operator!=(Vertex v);

    Point GetPoint(void);
    void SetPoint(Point p);

    // Dijkstra's algorithm specific methods
    void Dijkstra_Initialize(void);
    float Dijkstra_GetDist(void);
    bool Dijkstra_GetVisited(void);
    Vertex *Dijkstra_GetBefore(void);
    void Dijkstra_SetDist(float dist);
    void Dijkstra_SetVisited(bool visited);
    void Dijkstra_SetBefore(Vertex *before);

private:
    Point p;

    // Dijkstra's algorithm specific attributes
    float dij_dist;
    bool dij_visited;
    Vertex *dij_before;
};

#endif // VERTEX_H
