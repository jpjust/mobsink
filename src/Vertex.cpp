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

#include "Vertex.h"
#include <stdlib.h>

// Default constructor
Vertex::Vertex(void)
{
    // Does nothing
}

Vertex::Vertex(Point p)
{
    SetPoint(p);
}

// Operator ==
bool Vertex::operator==(Vertex v)
{
    return GetPoint() == v.GetPoint();
}

// Operator !=
bool Vertex::operator!=(Vertex v)
{
    return GetPoint() != v.GetPoint();
}

// Dijkstra's algorithm initialization
void Vertex::Dijkstra_Initialize(void)
{
    Dijkstra_SetBefore(NULL);
    Dijkstra_SetVisited(false);
    Dijkstra_SetDist((unsigned long int)(-1));
}

// Getters and setters
Point Vertex::GetPoint(void)
{
    return this->p;
}

void Vertex::SetPoint(Point p)
{
    this->p = p;
}

// Dijkstra's algorithm specific getters and setters
float Vertex::Dijkstra_GetDist(void)
{
    return this->dij_dist;
}

bool Vertex::Dijkstra_GetVisited(void)
{
    return this->dij_visited;
}

Vertex *Vertex::Dijkstra_GetBefore(void)
{
    return this->dij_before;
}

void Vertex::Dijkstra_SetDist(float dist)
{
    this->dij_dist = dist;
}

void Vertex::Dijkstra_SetVisited(bool visited)
{
    this->dij_visited = visited;
}

void Vertex::Dijkstra_SetBefore(Vertex *before)
{
    this->dij_before = before;
}
