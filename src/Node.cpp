/*
 * Nodes modeling for MobSink.
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

#include "Node.h"
#include "Cluster.h"
#include <stdio.h>

// Constructor
Node::Node(float x, float y, int RL, Cluster *cluster, Node *nexthop)
    :Point(x, y)
{
    SetCluster(cluster);
    SetNextHop(nexthop);
    hops_to_sink = 0;
    params_init.RL = RL;
    params_init.active = true;
    Reset();
}

// Destructor
Node::~Node(void)
{

}

// Getters
int Node::GetRL(void)
{
    return this->RL;
}

int Node::GetRunTimeRL(void)
{
    // If there is no data to send or no energy, the RunTime RL is zero
    if ((!HasData()) || (power == 0))
        return 0;
    else
        return this->RL;
}

Cluster *Node::GetCluster(void)
{
    return this->cluster;
}

Node *Node::GetNextHop(void)
{
    return this->nexthop;
}

int Node::GetHopsToSink(void)
{
    return this->hops_to_sink;
}

double Node::GetPDUs(void)
{
    return this->pdu;
}

double Node::GetDropped(void)
{
	return this->dropped;
}

double Node::GetPower(void)
{
    return this->power;
}

bool Node::IsActive(void)
{
    return ((this->active) && (this->power > 0));
}

// Setters
void Node::SetRL(int RL)
{
    this->RL = RL;
}

void Node::SetCluster(Cluster *cluster)
{
    this->cluster = cluster;
}

void Node::SetNextHop(Node *nexthop)
{
    this->nexthop = nexthop;
}

void Node::SetHopsToSink(int hops)
{
    this->hops_to_sink = hops;
}

// Reset node
void Node::Reset(void)
{
    this->hops_to_sink = 0;
    this->hops = 0;
    this->tx = 0;
    this->nexthop = NULL;
    this->power = POWER;
    this->time_elapsed = 0;
    this->pdu = 0;
    this->dropped = 0;
    this->changed = false;
    this->active = params_init.active;
    this->RL = params_init.RL;
}

// Compare links
// Compare the current link with the one to dest. If linking to dest
// is better, return true.
bool Node::CompareLinks(Node *dest)
{
    // If this node doesn't have a next hop yet, return true
    if (nexthop == NULL)
        return true;

    // If the new connection will make a shorter link (less hops)
    // or will keep the number of hops, but in a shorter distance, return true.
    if ((hops_to_sink > dest->GetHopsToSink() + 1) ||
        ((hops_to_sink == dest->GetHopsToSink() + 1) && (Distance(*nexthop) > Distance(*dest))))
        return true;

    // If none of the conditions above as satisfied, return false
    return false;
}

// Increase the buffer, simulating new data generation
void Node::GenerateData(void)
{
    if (IsActive())
    {
    	//if (pdu + pkt_rate[RL] <= PDU_BUFFER)
    	//	pdu += pkt_rate[RL];
    	//else
    	//	GetCluster()->IncreaseDrops(pkt_rate[RL]);
    	if (pdu + (RL * GEN_RATE) <= PDU_BUFFER)
    		pdu += RL * GEN_RATE;
    	else
    		GetCluster()->IncreaseDrops(RL * GEN_RATE);
    }
}

// Return if there is data to send
bool Node::HasData(void)
{
    if (pdu >= 1)
        return true;
    else
        return false;
}

// Return if this node has changed any control parameter and reset
// the flag
bool Node::HasChanged(void)
{
    if (changed)
    {
        changed = false;
        return true;
    }

    return false;
}

// Return the average of hops used for transmitting packets
double Node::GetHopsAvg(void)
{
    if (this->tx > 0)
        return this->hops / this->tx;
    else
        return 0;
}

// Increase the buffer and decrease energy, simulating
// data reception. Return how much energy was spent.
double Node::GetData(void)
{
    if (!IsActive())
        return 0;

    // If there is power to receive, go ahead
    if (power >= PWR)
    {
        power -= PWR;
        // Check for available space in buffer
        if ((pdu + 1) <= PDU_BUFFER)
        	pdu++;
        else // Drop packet
        	GetCluster()->IncreaseDrops(1);
        return PWR;
    }
    else    // Else, drain all energy and tell the sender
    {
        double pwr = power;
        power = 0;
        return pwr;
    }
}

// Decrease the buffer and energy, simulating data transmission.
// Return how much energy was spent.
double Node::SendData(void)
{
    if (!IsActive())
        return 0;

    // 1 second has passed...
    //time_elapsed++;
    double txtime = 1;

    // Energy consumed
    double energy = 0;

    // While there are time to transmit, do it
    while (txtime >= TX_TIME)
    {
        // Decrease the time needed to transmit 1 PDU
        txtime -= TX_TIME;

        // If there is no data to be sent or no next hop, quit
        if ((HasData() == false) || (nexthop == NULL))
            break;

        // If there is no power to transmit data, give up and
        // zero power
        if (power < PWT)
        {
            energy = power;
            power = 0;
        }
        else // If there is power to transmit, go ahead
        {
            // Get how much energy the next hop will spend receiving
            double pwr_hop = nexthop->GetData();

            // Check if transmission was done
            if (pwr_hop < 0)
            	return 0;

            // Increase hops and tx counters
            hops += hops_to_sink;
            tx++;

            // If the next hop got the data or it is the sink, compute
            if ((pwr_hop > 0) || (hops_to_sink == 1))
            {
                power -= PWT;
                pdu--;
                energy += PWT;

                // Add next hop's PWR
                energy += pwr_hop;
            }
        }
    }

    return energy;
}

// Make the node work for 1 second
void Node::Work(void)
{
    // Here we must check for any change in node control
    map<int, struct node_control_params>::iterator it = node_control.find(time_elapsed);
    if (it != node_control.end())
    {
        struct node_control_params p = it->second;
        SetRL(p.RL);
        this->active = p.active;
        this->changed = true;
    }

    // Only after changing control we increment time.
    // That way we can let the sensor SendData for the GeneratedData
    // in this method.
    GenerateData();
    time_elapsed++;
}

// Insert control settings at a specific time
void Node::InsertControl(int time, int RL, bool active)
{
    struct node_control_params p;
    p.RL = RL;
    p.active = active;
    this->node_control.insert(pair<int, struct node_control_params>(time, p));
}
