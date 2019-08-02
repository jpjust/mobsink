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

#include "Network.h"
#include "Cluster.h"
#include <cmath>

#include <stdio.h>

// Constructor
Network::Network()
{
	this->graph_ready = false;
	this->txrange = 0;
	this->static_sinks = false;
    this->sink_move_boundary = 0;
}

// Getters
vector<Cluster *> Network::GetClusters(void)
{
    return clusters;
}

vector<Node *> Network::GetNodes(void)
{
    return nodes;
}

vector<Obstacle> Network::GetObstacles(void)
{
    return obstacles;
}

vector<Path> Network::GetPaths(void)
{
    return paths;
}

float Network::GetRange(void)
{
    return txrange;
}

// Setters
void Network::SetRange(float range)
{
    this->txrange = range;
}

// Return the size of the network by using nodes positions
wxSize Network::GetSize(int &leftmost, int &rightmost, int &top, int &bottom)
{
    // Figure out the network size finding the edges coordinates
    float x[2], y[2];

    x[0] = x[1] = nodes.at(0)->GetX();
    y[0] = y[1] = nodes.at(0)->GetY();

    for (unsigned int i = 1; i < nodes.size(); i++)
    {
        if (nodes.at(i)->GetX() < x[0])
            x[0] = nodes.at(i)->GetX();

        if (nodes.at(i)->GetX() > x[1])
            x[1] = nodes.at(i)->GetX();

        if (nodes.at(i)->GetY() < y[0])
            y[0] = nodes.at(i)->GetY();

        if (nodes.at(i)->GetY() > y[1])
            y[1] = nodes.at(i)->GetY();
    }

    // Return boundaries
    leftmost = x[0];
    rightmost = x[1];
    top = y[0];
    bottom = y[1];

    // Calculate size and return
    int width = x[1] - x[0];
    int height = y[1] - y[0];

    return wxSize(width, height);
}

// Clear the clusters
void Network::ClearClusters(void)
{
    while (clusters.size() > 0)
    {
        delete clusters.back();
        clusters.pop_back();
    }
}

// Clear the network
void Network::Clear(void)
{
    this->sink_move_boundary = 0;
    ClearClusters();

    while (nodes.size() > 0)
    {
        delete nodes.back();
        nodes.pop_back();
    }

    obstacles.clear();
    paths.clear();
    G.Clear();
}

// Insert a new node into the nodes vector
void Network::InsertNode(Node *n)
{
    nodes.push_back(n);
}

// Insert a new obstacle into the obstacles vector
void Network::InsertObstacle(Obstacle o)
{
    obstacles.push_back(o);
}

// Insert a new path into the paths vector
void Network::InsertPath(Path p)
{
    paths.push_back(p);
    this->graph_ready = false;
}

// Create a new cluster and move its sink
void Network::InsertSink(Sink *s)
{
    Cluster *newcluster = new Cluster();
    newcluster->MoveSink(s->GetX(), s->GetY());
    clusters.push_back(newcluster);
}

// Return a list of nodes in a grid structure
vector<Node *> Network::GridNodes(int n, int RL, int width, int height, int x_start, int y_start)
{
    vector<Node *> nodes;

    // Number of sensors
    if (n < 1)
        return nodes;

    // Calculate how many nodes will be created in each direction
    int nodes_y = sqrt(n);
    int nodes_x = n / nodes_y;

    // If there are less nodes than solicited, add a new line for them
    if (nodes_x * nodes_y < n)
        nodes_y++;

    // Calculate the position offset
    int x_offset = width / nodes_x;
    int y_offset = height / nodes_y;

    // Insert nodes
    int created = 0;
    for (int j = 0; j < nodes_y; j++)
    {
        for (int i = 0; i < nodes_x; i++)
        {
            // All nodes were already created (this may happen if
            // was not possible to create a perfect rectangle)
            if (created == n)
                break;

            Node *node = new Node();
            node->SetX(x_start + (x_offset * (i + 1)) - (x_offset / 2));
            node->SetY(y_start + (y_offset * (j + 1)) - (y_offset / 2));
            node->SetRL(RL);

            nodes.push_back(node);

            created++;
        }
    }

    return nodes;
}

// Fill the map with sinks forming a grid
void Network::GridSinks(void)
{
    // If there are no nodes, quit
    if (nodes.size() == 0)
        return;

    // Get the list of sinks
    int x[2], y[2];
    wxSize netsize = GetSize(x[0], x[1], y[0], y[1]);
    vector<Node *> sinks = GridNodes(clusters.size(), 0, netsize.GetWidth(), netsize.GetHeight(), x[0], y[0]);

    // Insert sinks
    for (unsigned int k = 0; k < clusters.size(); k++)
        clusters.at(k)->MoveSink(sinks.at(k)->GetX(), sinks.at(k)->GetY());
}

// Create the clusters
void Network::CreateClusters(unsigned int n_sinks)
{
    ClearClusters();

    for (unsigned int k = 0; k < n_sinks; k++)
    {
        Cluster *newcluster = new Cluster();
        clusters.push_back(newcluster);
    }
}

// Update the cluster of each node
void Network::UpdateClusters(void)
{
    // Remove the nodes from the clusters and clean all links
    for (unsigned int k = 0; k < clusters.size(); k++)
    {
        clusters.at(k)->CleanLinks();
        clusters.at(k)->RemoveAll();
    }

    // If the sinks are static, every sensor is part of every cluster
    // Use the first cluster as reference for some calculations
    if (static_sinks)
    {
        for (unsigned int i = 0; i < nodes.size(); i++)
            for (unsigned int k = 0; k < clusters.size(); k++)
            {
                clusters.at(k)->InsertNode(nodes.at(i));
                nodes.at(i)->SetCluster(clusters.at(0));
            }

        return;
    }

    // For each node, check which mean is closest to it and insert it in that cluster
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        // If this node is not active, skip
        if (!nodes.at(i)->IsActive())
            continue;

        // The min_dist will be initialized with the distance between the node and the first mean
        float min_dist = nodes.at(i)->Distance(*clusters.at(0)->GetMean());
        nodes.at(i)->SetCluster(clusters.at(0));

        // Check if there is a nearest mean
        for (unsigned int k = 1; k < clusters.size(); k++)
        {
            float d = nodes.at(i)->Distance(*clusters.at(k)->GetMean());
            if (d < min_dist)
            {
                nodes.at(i)->SetCluster(clusters.at(k));
                min_dist = d;
            }
        }

        // Insert this node in the correct cluster
        nodes.at(i)->GetCluster()->InsertNode(nodes.at(i));
    }
}

// Initialize the means using static positioning
void Network::MeansInitializeStatic(void)
{
    static_sinks = true;
    sink_move_boundary = 0;
    GridSinks();
    UpdateClusters();
}

// Initialize the means using Forgy
void Network::MeansInitializeForgy(void)
{
    // Initialization
    bool node_used[nodes.size()];
    static_sinks = false;

    for (unsigned int i = 0; i < nodes.size(); i++)
        node_used[i] = false;

    // Choose a random node and set its position as the mean
    // position of each cluster
    for (unsigned int k = 0; k < clusters.size(); k++)
    {
        int n;

        do
        {
            n = rand() % nodes.size();
        }
        while (node_used[n]);

        Cluster *cur = clusters.at(k);
        cur->GetMean()->SetX(nodes.at(n)->GetX());
        cur->GetMean()->SetY(nodes.at(n)->GetY());

        node_used[n] = true;
    }
}

// Initialize the means using Random Partition
void Network::MeansInitializeRandom(void)
{
    // Initialization
    static_sinks = false;

    // Distribute the nodes to the clusters
    for (unsigned int i = 0; i < nodes.size(); i++)
    {
        int cluster = rand() % clusters.size();
        clusters.at(cluster)->InsertNode(nodes.at(i));
        nodes.at(i)->SetCluster(clusters.at(cluster));
    }

    // Set means initial positions
    for (unsigned int k = 0; k < clusters.size(); k++)
        clusters.at(k)->UpdateMean();
}

// Initialize the means using Just
// This algorithm visit every node in the network and insert it
// into a cluster. After visiting a node, the next one to go is
// the farest not visited node. Nodes with RL = 0 are not considered.

// RLBASED: the algorithm will use a scoring mechanism to determine
//          the next node. The score equals to distance * RL

void Network::MeansInitializeJust(void)
{
    static_sinks = false;

    // Insert sinks in an uniform manner
    GridSinks();

    // Place each sink in its nearest not used sensor
    bool node_used[nodes.size()];
    for (unsigned int i = 0; i < nodes.size(); i++)
        node_used[i] = false;

    for (unsigned int k = 0; k < clusters.size(); k++)
    {
        // Initialize
        unsigned int i = 0;

        // Don't re-use nodes and don't use inactive nodes
        while ((i < nodes.size()) && ((node_used[i] == true) || (!nodes.at(i)->IsActive())))
            i++;

        // If we couldn't find any active sensor, let's try with inactive
        if (i >= nodes.size())
        {
			i = 0;
			while (node_used[i] == true)
				i++;
        }

        Sink *cur = clusters.at(k)->GetSink();
        float min_dist = cur->Distance(*nodes.at(i));
        int selected = i;

        for (i++; i < nodes.size(); i++)
        {
            if ((node_used[i] == true) || (!nodes.at(i)->IsActive()))
                continue;

            float cur_dist = cur->Distance(*nodes.at(i));
            if (cur_dist < min_dist)
            {
                min_dist = cur_dist;
                selected = i;
            }
        }

        // Move the sink and mark the sensor as used
        clusters.at(k)->MoveSink(nodes.at(selected)->GetX(), nodes.at(selected)->GetY());
        node_used[selected] = true;
    }

    // Update clusters and balance means positions
    UpdateClusters();
    BalanceMeans();
    return;
}

// Run the k-means clustering algorithm to set the means until its positions no longer change
void Network::RunKmeans(void)
{
    float delta_pos;

    // Run k-means
    do
    {
        UpdateClusters();
        delta_pos = 0;

        for (unsigned int k = 0; k < clusters.size(); k++)
            delta_pos += clusters.at(k)->UpdateMeanDaniel(GetRange());
        printf("GetRange() = %f\tdelta_pos = %f\n", GetRange(), delta_pos);
    }
    while (delta_pos >= 1);
}

// Position the sinks and return whenever any of them has changed (use current time to calculate Dijkstra's path)
bool Network::PositionSinks(bool use_runtime_RL, int sinkpos, int t, bool use_traffic)
{
    // Variables
    int x[2], y[2];
    bool hasChanged = false;
    vector<Point> prev_pos;

    switch (sinkpos)
    {
    case SINKPOS_STATIC:
        // Don't touch the sinks if they are static
        UpdateClusters();

        // Check if the sink is in a permitted area
        for (unsigned int k = 0; k < clusters.size(); k++)
        {
            CheckObstacles(k);
            CheckPaths(k);
        }

        return false;
        break;

    case SINKPOS_VERTICAL:
        // Move each sink by SINK_SPEED pixels in vertical
        GetSize(x[0], x[1], y[0], y[1]);
        for (unsigned int k = 0; k < clusters.size(); k++)
        {
            Sink *s = clusters.at(k)->GetSink();

            // Check if the sink has already reached a boundary
            if (s->GetY() >= y[1])
                sink_move_boundary = 0;

            if (s->GetY() <= y[0])
                sink_move_boundary = 1;

            // Move sink
            clusters.at(k)->WalkSink(Point(s->GetX(), y[sink_move_boundary]));
        }
        UpdateClusters();
        return true;
        break;

    case SINKPOS_HORIZONTAL:
        // Move each sink by SINK_SPEED pixels in horizontal
        GetSize(x[0], x[1], y[0], y[1]);
        for (unsigned int k = 0; k < clusters.size(); k++)
        {
            Sink *s = clusters.at(k)->GetSink();

            // Check if the sink has already reached a boundary
            if (s->GetX() >= x[1])
                sink_move_boundary = 0;

            if (s->GetX() <= x[0])
                sink_move_boundary = 1;

            // Move sink
            clusters.at(k)->WalkSink(Point(x[sink_move_boundary], s->GetY()));
        }
        UpdateClusters();
        return true;
        break;

    case SINKPOS_RLFORGY:
    case SINKPOS_RLRANDOM:
    case SINKPOS_RLJUST:
        // Store the current (or previous) positions of every sink
        for (unsigned int k = 0; k < clusters.size(); k++)
            prev_pos.push_back(*clusters.at(k)->GetSink());

        // Initialize means
        switch (sinkpos)
        {
        case SINKPOS_RLFORGY:
            MeansInitializeForgy();
            break;

        case SINKPOS_RLRANDOM:
            MeansInitializeRandom();
            break;

        default:
            MeansInitializeJust();
        }

        // First, run k-means to set the clusters
        RunKmeans();

        // Then, for each cluster, position the sink and store its path using Dijkstra's algorithm
        for (unsigned int k = 0; k < clusters.size(); k++)
        {
            Vertex *src = G.InsertVertexAndConnect(prev_pos.at(k));
            clusters.at(k)->MoveSinkDaniel(use_runtime_RL, GetRange());

            // To make sure no sink will be disconnected, we create the
            // links and verify if there is any disconnected sink. If we find
            // a disconnected sink, we move it to the nearest disconnected node
            clusters.at(k)->CreateLinks(txrange);
            Sink *s = clusters.at(k)->GetSink();
            if (s->GetConnected() == false)
            {
                Node *n = clusters.at(k)->FindNearestNode(*s);
                if (n != NULL)
                {
                    s->SetX(n->GetX());
                    s->SetY(n->GetY());
                }
            }

            // Check if the sink is in a permitted area
            CheckObstacles(k);
            CheckPaths(k);

            // Update graph
            Vertex *dst = G.InsertVertexAndConnect(*clusters.at(k)->GetSink());

            // Get the distance from the new position to the old one
            float delta_post = dst->GetPoint().Distance(src->GetPoint());

            // Check if sink has moved
            if (*src != *dst)
            {
                // If there are restricted paths, use Dijkstra's distance
                // Otherwise, use moved distance
            	if (paths.size() > 0)
            	{
            		clusters.at(k)->SetLastPath(G.Dijkstra(src, dst, t, use_traffic));
            		if ((t == 0) || (dst->Dijkstra_GetVisited()))
            		{
						clusters.at(k)->IncrementDist(dst->Dijkstra_GetDist());
						clusters.at(k)->StopSinkUntil(t + dst->Dijkstra_GetTime());
						clusters.at(k)->AddTravel(dst->Dijkstra_GetTime());
		                hasChanged = true;
            		}
            		else
            		{
            			// After runing Dijkstra's algorithm, we detect an impossible route
            			// In this case, this movement is not allowed
            			clusters.at(k)->MoveSink(prev_pos.at(k).GetX(), prev_pos.at(k).GetY());
            		}
            	}
                else
                {
                    clusters.at(k)->IncrementDist(delta_post);
                    hasChanged = true;
                }
            }
        }

        return hasChanged;
        break;

    default:
        UpdateClusters();
        return false;
    }
}

// Return the nearest Cluster from Point p (based on its Sink position)
Cluster *Network::GetNearestClusterFrom(Point p)
{
    if (clusters.size() == 0)
        return NULL;

    // Calculates the distance to the first sink
    float min_dist = p.Distance(*clusters.at(0)->GetSink());
    int id = 0;

    // Check for a nearest sink
    for (unsigned int k = 1; k < clusters.size(); k++)
    {
        float d = p.Distance(*clusters.at(k)->GetSink());
        if (d < min_dist)
        {
            min_dist = d;
            id = k;
        }
    }

    return clusters.at(id);
}

// Run the CreateLinks() method in all clusters
void Network::CreateLinks(void)
{
    for (unsigned int k = 0; k < clusters.size(); k++)
        clusters.at(k)->CreateLinks(txrange);
}

// Balance means positions
void Network::BalanceMeans(void)
{
    // If there are no clusters, quit
    if (clusters.size() == 0)
        return;

    // Parameters
    int totalRL = 0;

    // Get network total RL
    for (unsigned int i = 0; i < nodes.size(); i++)
        totalRL += nodes.at(i)->GetRL();

    // If network total RL is 0, just quit balancing
    if (totalRL == 0)
        return;

    // For each cluster, move its mean accordingly to other
    // clusters total RL
    for (unsigned int k = 0; k < clusters.size(); k++)
    {
        for (unsigned int l = 0; l < clusters.size(); l++)
        {
            // Don't compare with itself
            if (k == l)
                continue;

            // Calculate new position
            float new_x, new_y;

            // How much to move in each axis
            new_x = clusters.at(l)->GetMean()->GetX() - clusters.at(k)->GetMean()->GetX();
            new_y = clusters.at(l)->GetMean()->GetY() - clusters.at(k)->GetMean()->GetY();

            // Make proportional to clusters' RL
            new_x *= clusters.at(l)->GetRL() / totalRL;
            new_y *= clusters.at(l)->GetRL() / totalRL;

            // Add the offset
            new_x += clusters.at(k)->GetMean()->GetX();
            new_y += clusters.at(k)->GetMean()->GetY();

            // Move
            clusters.at(k)->MoveSink(new_x, new_y);
        }
    }
}

// Check if there are any sink inside an obstacle and move it out
/*
1) Calculates the proportion between the radius of the obstacle and the distance
   from its center to the sink:

   p = r/d

2) Multiplies p by the delta on X and Y axes (sink coordinates minus center
   coordinates):

   X = (xa - xc) * p + xc
   Y = (ya - yc) * p + yc

   X, Y  : final coordinates (border of the circunference)
   xa, ya: former coordinates of the sink
   xc, yc: coordinates of the obstacle
 */
void Network::CheckObstacles(int k)
{
    float dist;

    // If there are no obstacles, just quit
    if (obstacles.empty())
        return;

    for (unsigned int i = 0; i < obstacles.size(); i++)
    {
        // Verify if the sink is inside an obstacle
        dist = obstacles[i].Distance(*clusters.at(k)->GetSink());
        if (dist < obstacles[i].GetRadius())
        {
            // Calculate a new position for this sink
            float prop = obstacles[i].GetRadius() / dist;
            float new_x = (clusters.at(k)->GetSink()->GetX() - obstacles.at(i).GetX()) * prop + obstacles.at(i).GetX();
            float new_y = (clusters.at(k)->GetSink()->GetY() - obstacles.at(i).GetY()) * prop + obstacles.at(i).GetY();
            clusters.at(k)->MoveSink(new_x, new_y);
        }
    }
}

// Move the sinks to its nearest allowed path
void Network::CheckPaths(int k)
{
    float dist, aux_dist;
    Point new_s, aux_s;

    // If there are no paths, just quit
    if (paths.empty())
        return;

    // Initialize variables using first path
    new_s = paths.at(0).GetNearestPoint(*clusters.at(k)->GetSink());
    dist = clusters.at(k)->GetSink()->Distance(new_s);

    for (unsigned int i = 1; i < paths.size(); i++)
    {
        // Verify if the distance to the next path is lower
        aux_s = paths.at(i).GetNearestPoint(*clusters.at(k)->GetSink());
        aux_dist = clusters.at(k)->GetSink()->Distance(aux_s);

        if (aux_dist < dist)
        {
            dist = aux_dist;
            new_s = aux_s;
        }
    }

    // Move the sink to its new position
    clusters.at(k)->MoveSink(new_s.GetX(), new_s.GetY());
}

// Build a graph using network objects (sinks and paths)
void Network::BuildGraph(void)
{
	if (this->graph_ready)
		return;

    // Clear the graph
    ClearGraph();

    // For each path, add its points as vertices
    for (unsigned int i = 0; i < paths.size(); i++)
    {
        Vertex *v = G.InsertVertex(paths.at(i).GetPointA());
        Vertex *w = G.InsertVertex(paths.at(i).GetPointB());

        switch (paths.at(i).GetFlow())
        {
        case PATHFLOW_AB:
        	G.InsertEdge(v, w, paths.at(i).GetPathControl(), paths.at(i).GetSpeedLimit());
        	break;

        case PATHFLOW_BA:
        	G.InsertEdge(w, v, paths.at(i).GetPathControl(), paths.at(i).GetSpeedLimit());
        	break;

        case PATHFLOW_BI:
        	G.InsertEdge(v, w, paths.at(i).GetPathControl(), paths.at(i).GetSpeedLimit());
        	G.InsertEdge(w, v, paths.at(i).GetPathControl(), paths.at(i).GetSpeedLimit());
        }
    }

    // For each intersection, add its position as a vertex
    bool exist;
    for (unsigned int i = 0; i < paths.size(); i++)
    {
        for (unsigned int j = 0; j < paths.size(); j++)
        {
            // Skip if both Paths are the same
            if (i == j) continue;

            // Check if there is an intersection
            Point p = paths.at(i).GetIntersection(paths.at(j), exist);
            if (exist)
                G.InsertVertexAndConnect(p);
        }
    }

    // For each sink, add its position as a vertex
    for (unsigned int k = 0; k < clusters.size(); k++)
        G.InsertVertexAndConnect(*clusters.at(k)->GetSink());

#ifdef DEBUG
    printf("\nBuilding graph...\n\n");

    // Print all the vertices
    vector<Vertex *> v = G.GetVertices();

    if (v.size() > 0)
    {
        printf("V = {(%.0f, %.0f)", v[0]->GetPoint().GetX(), v[0]->GetPoint().GetY());
        for (unsigned int i = 1; i < v.size(); i++)
            printf(", (%.0f, %.0f)", v[i]->GetPoint().GetX(), v[i]->GetPoint().GetY());
        printf("}\n\n");
    }

    // Print all the edges
    vector<Edge *> e = G.GetEdges();

    if (e.size() > 0)
    {
        printf("E = {\n\t{(%.0f, %.0f), (%.0f, %.0f)}",
            e[0]->GetSource()->GetPoint().GetX(), e[0]->GetSource()->GetPoint().GetY(),
            e[0]->GetDestination()->GetPoint().GetX(), e[0]->GetDestination()->GetPoint().GetY());

        for (unsigned int i = 1; i < e.size(); i++)
        printf(",\n\t{(%.0f, %.0f), (%.0f, %.0f)}",
            e[i]->GetSource()->GetPoint().GetX(), e[i]->GetSource()->GetPoint().GetY(),
            e[i]->GetDestination()->GetPoint().GetX(), e[i]->GetDestination()->GetPoint().GetY());
        printf("\n}\n\n");
    }

    // Print number of vertices and edges
    printf("Vertices: %lu\n", v.size());
    printf("Edges: %lu\n\n", e.size());
    printf("Graph building finished!\n\n");
#endif // DEBUG

    this->graph_ready = true;
}

// Clear the graph previously built
void Network::ClearGraph(void)
{
    G.Clear();
    this->graph_ready = false;
}
