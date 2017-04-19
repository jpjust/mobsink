/*
 * Graph implementation for MobSink.
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

#include "Graph.h"
#include "Path.h"
#include <stdio.h>

// Default constructor
Graph::Graph()
{
    // Does nothing
}

// Getters and setters
vector<Vertex *> Graph::GetVertices(void)
{
    return this->vertices;
}

vector<Edge *> Graph::GetEdges(void)
{
    return this->edges;
}

// Return the index of a vertex or -1 if it doesn't exist
int Graph::FindVertex(Vertex v)
{
    for (unsigned int i = 0; i < vertices.size(); i++)
        if (*vertices.at(i) == v)
            return i;

    return -1;
}

// Check if there is a Vertex v
bool Graph::HasVertex(Vertex v)
{
    if (FindVertex(v) > -1)
        return true;

    return false;
}

// Insert a new vertex
Vertex *Graph::InsertVertex(Point p)
{
    Vertex *v = new Vertex(p);
    int i;

    if ((i = FindVertex(*v)) == -1)
    {
        vertices.push_back(v);
        return v;
    }
    else
    {
        delete v;   // We won't need v
        return vertices.at(i);
    }
}

// Insert a new vertex and connect it to other(s) if it is
// positioned on an edge
Vertex *Graph::InsertVertexAndConnect(Point p)
{
    // Insert the point as a new vertex and stores it in y
    Vertex *y = InsertVertex(p);

    // Find a previously added edged that has this point on it
    // (we make a copy of the edges to not alter them yet)
    vector<Edge *> edges = GetEdges();
    for (unsigned k = 0; k < edges.size(); k++)
    {
        // If v is already a vertex of this edge, skip
        if ((*edges.at(k)->GetSource() == *y) || (*edges.at(k)->GetDestination() == *y))
            continue;

        Path r(edges.at(k)->GetSource()->GetPoint(), edges.at(k)->GetDestination()->GetPoint());

        // This edge has point p on it?
        if (r.HasPoint(p))
        {
#ifdef DEBUG
            // Print information about the current intersection
            printf("(%.0f. %.0f) -> {(%.0f, %.0f), (%.0f, %.0f)}\n\n",
                   p.GetX(), p.GetY(),
                   edges.at(k)->GetSource()->GetPoint().GetX(), edges.at(k)->GetSource()->GetPoint().GetY(),
                   edges.at(k)->GetDestination()->GetPoint().GetX(), edges.at(k)->GetDestination()->GetPoint().GetY());
#endif // DEBUG

            // Get the two vertices that form this edge
            Vertex *v = edges.at(k)->GetSource();
            Vertex *w = edges.at(k)->GetDestination();

            // At last, delete the previously added edged formed by (v, w) and add the edges
            // formed by (v, y) and (y, w);
            DeleteEdge(*edges.at(k));
            InsertEdge(v, y);
            InsertEdge(y, w);
        }
    }

    return y;
}

// Delete a vertex from the vector and return true if it suceeded
bool Graph::DeleteVertex(Vertex v)
{
    int p = FindVertex(v);

    if (p > -1)
    {
        Vertex *vptr = vertices.at(p);
        vertices.erase(vertices.begin() + p);
        delete vptr;
        return true;
    }

    return false;
}

// Return the index of an Edge or -1 if it doesn't exist
int Graph::FindEdge(Edge e)
{
    for (unsigned int i = 0; i < edges.size(); i++)
        if (*edges.at(i) == e)
            return i;

    return -1;
}

// Check if there is an Edge e
bool Graph::HasEdge(Edge e)
{
    if (FindEdge(e) > -1)
        return true;

    return false;
}

// Insert a new edge
Edge *Graph::InsertEdge(Vertex *src, Vertex *dst, int weight)
{
    Edge *e = new Edge(src, dst);
    int i;

    if ((i = FindEdge(*e)) == -1)
    {
        edges.push_back(e);
        return e;
    }
    else
    {
        delete e;   // We won't need e
        return edges.at(i);
    }
}

// Delete an Edge from the vector and return true if it suceeded
bool Graph::DeleteEdge(Edge e)
{
    int p = FindEdge(e);

    if (p > -1)
    {
        Edge *eptr = edges.at(p);
        edges.erase(edges.begin() + p);
        delete eptr;
        return true;
    }

    return false;
}

// Clear the Graph
void Graph::Clear(void)
{
    while (!edges.empty())
    {
        Edge *e = edges.back();
        edges.pop_back();
        delete e;
    }

    while (!vertices.empty())
    {
        Vertex *v = vertices.back();
        vertices.pop_back();
        delete v;
    }
}

// Run Dijkstra's algorithm to find the best path from src to dst
vector<Point> Graph::Dijkstra(Vertex *src, Vertex *dst)
{
    vector<Point> path;
    if ((src == NULL) || (dst == NULL) || (src == dst))
        return path;

    // Build an adjacency list for the graph
    vector<Vertex *> adj[vertices.size()];
    int cur = 0;
    int vleft = vertices.size();

    // Create an adjacency list
    for (unsigned int i = 0; i < vertices.size(); i++)
    {
        vertices.at(i)->Dijkstra_Initialize();

        // Check if the current vertex is the src
        if (*vertices.at(i) == *src)
            cur = i;

        // Check its edges
        for (unsigned int j = 0; j < edges.size(); j++)
        {
            // Check if the current vertex is part of the current edge
            if (*edges.at(j)->GetSource() == *vertices.at(i))
                adj[i].push_back(edges.at(j)->GetDestination());
            else if (*edges.at(j)->GetDestination() == *vertices.at(i))
                adj[i].push_back(edges.at(j)->GetSource());
        }
    }

    // Initialize vertices

    vertices.at(cur)->Dijkstra_SetDist(0);

    // Run Dijkstra
    float new_dist;
    while ((dst->Dijkstra_GetVisited() == false) && (vleft > 0))
    {
        vleft--;

        // Visit all neighbors of cur and update the distances
        for (unsigned int i = 0; i < adj[cur].size(); i++)
        {
            // Don't check against visited vertices
            if (adj[cur].at(i)->Dijkstra_GetVisited() == true)
                continue;

            // Compare the distances
            new_dist = vertices.at(cur)->Dijkstra_GetDist() + vertices.at(cur)->GetPoint().Distance(adj[cur].at(i)->GetPoint());
            if (new_dist < adj[cur].at(i)->Dijkstra_GetDist())
            {
                // The new path is better. Update!
                adj[cur].at(i)->Dijkstra_SetDist(new_dist);
                adj[cur].at(i)->Dijkstra_SetBefore(vertices.at(cur));
            }
        }

        // Set the current vertex as visited and choose another one
        vertices.at(cur)->Dijkstra_SetVisited(true);

        // The new current vertex must be the unvisited one with the lower distance
        new_dist = (unsigned long int)(-1);
        for (unsigned int i = 0; i < vertices.size(); i++)
        {
            Vertex *v = vertices.at(i);
            if ((v->Dijkstra_GetVisited() == false) && (v->Dijkstra_GetDist() < new_dist))
            {
                cur = i;
                new_dist = v->Dijkstra_GetDist();
            }
        }
    }

    // Return the set of Edges using Graph::InsertEdge() to find them
    Vertex *v = dst;

    while (v != NULL)
    {
        path.push_back(v->GetPoint());
        v = v->Dijkstra_GetBefore();
    }

    return path;
}
