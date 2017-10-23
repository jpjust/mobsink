/*
 * Graph's Vertex class for MobSink.
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

#include "Vertex.h"
#include <stdlib.h>
#include <limits>

// Default constructor
Vertex::Vertex(void)
{
    Dijkstra_Initialize();
}

Vertex::Vertex(Point p)
{
	Dijkstra_Initialize();
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
    Dijkstra_SetDist(0);
    Dijkstra_SetWeight(std::numeric_limits<float>::max());
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

vector<Edge *> Vertex::GetNeightboors(void)
{
	return this->adj;
}

// Return the index of an Edge or -1 if it doesn't exist
int Vertex::FindEdge(Edge e)
{
    for (unsigned int i = 0; i < adj.size(); i++)
        if (*adj.at(i) == e)
            return i;

    return -1;
}

// Check if there is an Edge e
bool Vertex::HasEdge(Edge e)
{
    if (FindEdge(e) > -1)
        return true;

    return false;
}

// Insert a new edge (neighbor)
bool Vertex::InsertEdge(Edge *e)
{
	if (HasEdge(*e))
		return false;	// This edge is already present

	this->adj.push_back(e);
	return true;
}

// Delete an edge from the neighbors list
bool Vertex::DeleteEdge(Edge e)
{
    int p = FindEdge(e);

    if (p > -1)
    {
        adj.erase(adj.begin() + p);
        return true;
    }

    return false;
}

// Dijkstra's algorithm specific getters and setters
float Vertex::Dijkstra_GetWeight(void)
{
    return this->dij_weight;
}

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

void Vertex::Dijkstra_SetWeight(float weight)
{
	this->dij_weight = weight;
}

void Vertex::Dijkstra_SetVisited(bool visited)
{
    this->dij_visited = visited;
}

void Vertex::Dijkstra_SetBefore(Vertex *before)
{
    this->dij_before = before;
}
