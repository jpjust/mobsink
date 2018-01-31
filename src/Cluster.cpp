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

#include "Cluster.h"
#include <deque>
#include <cmath>

// Constructor
Cluster::Cluster()
{
    this->sink.SetX(0);
    this->sink.SetY(0);
    this->mean.SetX(0);
    this->mean.SetY(0);
    this->RL = 0;
    this->received_pdus = 0;
    this->dropped_pdus = 0;
    this->sink.SetCluster(this);
    this->moved_pixels = 0;
}

// Getters
Sink *Cluster::GetSink(void)
{
    return &this->sink;
}

Point *Cluster::GetMean(void)
{
    return &this->mean;
}

int Cluster::GetRL(void)
{
    return this->RL;
}

double Cluster::GetPDUs(void)
{
    return this->received_pdus;
}

double Cluster::GetDrops(void)
{
	return this->dropped_pdus;
}

float Cluster::GetMovedPixels(void)
{
    return this->moved_pixels;
}

vector<Point> Cluster::GetLastPath(void)
{
    return this->last_path;
}

// Sets the last path
void Cluster::SetLastPath(vector<Point> last_path)
{
    this->last_path = last_path;
}

// Increase the number of received PDUs by the sink
void Cluster::IncreasePDUs(void)
{
	if (SinkCanReceive())
		this->received_pdus++;
}

// Increase the number of dropped PDUs in this cluster
void Cluster::IncreaseDrops(double packets)
{
	this->dropped_pdus += packets;
}

// Reset the PDUs counting
void Cluster::ResetPDUs(void)
{
    this->received_pdus = 0;
    this->dropped_pdus = 0;
}

// Return the number of nodes
unsigned int Cluster::GetNodesCount(void)
{
    return nodes.size();
}

// Return the node at position i
Node *Cluster::GetNodeAt(unsigned int i)
{
    if ((i >= 0) && (i < nodes.size()))
        return nodes.at(i);
    else
        return NULL;
}

// Insert a node the the vector in this cluster
void Cluster::InsertNode(Node *node)
{
    // Don't accept dead nodes
    if ((node != NULL) && (node->GetPower() > 0))
    {
        nodes.push_back(node);
        RL += node->GetRL();
    }
}

// Remove a node in the position i
void Cluster::RemoveNodeAt(unsigned int i)
{
    if ((i >= 0) && (i < nodes.size()))
    {
        RL -= nodes.at(i)->GetRL();
        nodes.erase(nodes.begin() + i);
    }
}

// Remove a node in the position i and delete it from memory
void Cluster::DeleteNodeAt(unsigned int i)
{
    if ((i >= 0) && (i < nodes.size()))
    {
        RL -= nodes.at(i)->GetRL();
        delete nodes.at(i);
        nodes.erase(nodes.begin() + i);
    }
}

// Remove all nodes
void Cluster::RemoveAll(void)
{
    nodes.clear();
    RL = 0;
}

// Remove all nodes and delete them from memory
void Cluster::DeleteAll(void)
{
    while (nodes.size() > 0)
    {
        delete nodes.back();
        nodes.pop_back();
    }

    RL = 0;
}

// Set the new position of the mean
void Cluster::SetMean(float x, float y)
{
    mean.SetX(x);
    mean.SetY(y);
}

// Move the sink to a specific position
void Cluster::MoveSink(float x, float y)
{
    sink.SetX(x);
    sink.SetY(y);
    SetMean(x, y);
}

// Calculate position of the sink for this Cluster using Daniel's algorithm
float Cluster::MoveSinkDaniel(bool use_runtime_RL)
{
    // Initialization
    int RL;
    int totalRL = 0;
    float new_x = 0;
    float new_y = 0;
    float delta_pos;

    // For each node in the cluster, sum its position and relevance
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        Node *n = nodes.at(i);

        if (use_runtime_RL)
            RL = n->GetRunTimeRL();
        else
            RL = n->GetRL();

        new_x += n->GetX() * RL;
        new_y += n->GetY() * RL;
        totalRL += RL;
    }

    // If total relevance is zero, don't need to move sink
    if (totalRL == 0)
        return 0;

    // Divide the coordinates by the total relevance and move the sink
    new_x /= totalRL;
    new_y /= totalRL;

    // Calculate how much it has changed
    delta_pos = sqrt(pow(new_x - sink.GetX(), 2) + pow(new_y - sink.GetY(), 2));

    // Move the sink and the mean
    MoveSink(new_x, new_y);
    return delta_pos;
}

// Makes sink walk
float Cluster::WalkSink(Point dest)
{
    float dist = sink.Walk(dest);
    moved_pixels += dist;
    return dist;
}

// Update the mean of this Cluster by centralizing it and
// return how much it has moved
float Cluster::UpdateMean(void)
{
    // Position change
    float delta_pos = 0;

    // Calculate its new position
    float x = 0, y = 0, total = 0, new_x, new_y;

    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        // Don't consider nodes with RL = 0
        if (nodes.at(i)->GetRL() == 0)
            continue;

        x += nodes.at(i)->GetX();
        y += nodes.at(i)->GetY();
        total++;
    }

    // If there is no nodes in this Cluster, set total = 1 so the
    // code will set a position (0, 0) for this mean
    if (total < 1)
        total = 1;

    // New coordinates for the mean
    new_x = x / total;
    new_y = y / total;

    // Calculates how much it has changed
    delta_pos = sqrt(pow(new_x - mean.GetX(), 2) + pow(new_y - mean.GetY(), 2));

    // Set the mean new position
    mean.SetX(new_x);
    mean.SetY(new_y);

    return delta_pos;
}

// Calculate position of the mean for this Cluster using Daniel's algorithm
float Cluster::UpdateMeanDaniel(void)
{
    // Initialization
    int RL;
    int totalRL = 0;
    float new_x = 0;
    float new_y = 0;
    float delta_pos;

    // For each node in the cluster, sum its position and relevance
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        Node *n = nodes.at(i);

        // Don't consider nodes with RL = 0
        if (n->GetRL() == 0)
            continue;

        RL = n->GetRL();
        new_x += n->GetX() * RL;
        new_y += n->GetY() * RL;
        totalRL += RL;
    }

    // If total relevance is zero, don't need to move sink
    if (totalRL == 0)
        return 0;

    // Divide the coordinates by the total relevance and move the sink
    new_x /= totalRL;
    new_y /= totalRL;

    // Calculate how much it has changed and update moved_pixels
    delta_pos = sqrt(pow(new_x - mean.GetX(), 2) + pow(new_y - mean.GetY(), 2));

    // Move the mean
    mean.SetX(new_x);
    mean.SetY(new_y);

    return delta_pos;
}

// Remove all links in this cluster
void Cluster::CleanLinks(void)
{
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        nodes.at(i)->SetNextHop(NULL);
        nodes.at(i)->SetHopsToSink(0);
    }
}

// Create hop-by-hop links between the nodes
void Cluster::CreateLinks(float range)
{
    // Starting from the Sink, the algorithm will connect it
    // to every sensor within its transmission range. Then,
    // for each connected sensor, this procedure is repeated
    // until all sensors are visited (depending on the range,
    // not all sensors will be connected).
    sink.SetConnected(false);

    // Nodes that need to be visited
    deque<Node *> visit_queue;

    // Remove any previous NextHop link and connect the Sink
    // with all sensors within its range
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        if ((nodes.at(i)->Distance(sink) <= range) && (nodes.at(i)->GetPower() > 0))
        {
            if (nodes.at(i)->CompareLinks(&sink))
            {
                nodes.at(i)->SetNextHop(&sink);
                nodes.at(i)->SetHopsToSink(1);
                visit_queue.push_back(nodes.at(i));
                sink.SetConnected(true);
            }
        }
    }

    // While there are nodes to visit (nodes that were connected
    // but not checked for neighbors yet), visit each one and
    // connected them to unconnected nodes within its range.
    while (visit_queue.size() > 0)
    {
        // Get the first node from the vector
        Node *cur = visit_queue.front();
        visit_queue.pop_front();

        // Connect this node with all sensors within its range.
        // If the sensor is already connected, but can change to a
        // nearest route, reconnect it.
        for (unsigned int i = 0; i < nodes.size(); i++)
        {
            // Check if it is within its range
            if ((nodes.at(i)->Distance(*cur) <= range) && (nodes.at(i)->GetPower() > 0))
            {
                // Check if it hasn't a next hop or this new route is
                // better than the current
                if (nodes.at(i)->CompareLinks(cur))
                {
                    nodes.at(i)->SetNextHop(cur);
                    nodes.at(i)->SetHopsToSink(cur->GetHopsToSink() + 1);
                    visit_queue.push_back(nodes.at(i));
                }
            }
        }
    }
}

// Increments dist to moved_pixels (used when moving sink manually
// instead of walking)
void Cluster::IncrementDist(float dist)
{
    this->moved_pixels += dist;
}

// Return a pointer to the nearest Node from Point p
Node *Cluster::FindNearestNode(Point p)
{
    if (nodes.size() == 0)
        return NULL;

    int min_dist = p.Distance(*nodes.at(0));
    Node *n = nodes.at(0);

    for (unsigned int i = 1; i < nodes.size(); i++)
    {
        if (p.Distance(*nodes.at(i)) < min_dist)
        {
            min_dist = p.Distance(*nodes.at(i));
            n = nodes.at(i);
        }
    }

    return n;
}

// Informs the cluster of the current time
void Cluster::SetCurrentTime(unsigned int t)
{
	this->cur_time = t;
}

// Stop sink from receiving PDUs until time t seconds.
// This is done to simulate the time the sink is still moving to its final
// destination.
void Cluster::StopSinkUntil(unsigned int t)
{
	this->stopped_until = t;
}

// Return true if the sink can receive PDUs (look at stopped_until)
bool Cluster::SinkCanReceive(void)
{
	return (this->cur_time >= this->stopped_until);
}
