/*
 * Sink class for MobSink.
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

#include "Sink.h"
#include "Cluster.h"
#include <cmath>

// Constructor
Sink::Sink(float x, float y, int RL, Cluster *cluster, Node *nexthop)
    :Node(x, y, RL, cluster, nexthop)
{
    this->connected = false;
}

// Getter and setter
bool Sink::GetConnected(void)
{
    return this->connected;
}

void Sink::SetConnected(bool connected)
{
    this->connected = connected;
}

// Increase the buffer and account this transmission
double Sink::GetData(void)
{
    // Account the transmission
    GetCluster()->IncreasePDUs();

    // Sink has no energy constraints
    return 0;
}

// Make sink walk
float Sink::Walk(Point dest)
{
    // Calculate distance:speed ratio
    float ds_ratio = SINK_SPEED / Distance(dest);

    // Move proportionally
    float new_x = GetX() + (dest.GetX() - GetX()) * ds_ratio;
    float new_y = GetY() + (dest.GetY() - GetY()) * ds_ratio;
    float dist = sqrt(powf(abs(new_x - GetX()), 2) + powf(abs(new_y - GetY()), 2));

    SetX(new_x);
    SetY(new_y);

    // Return how much it has walked
    return dist;
}

