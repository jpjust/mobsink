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

#include <stdlib.h>
#include <limits>
#include "Edge.h"

// Default constructor
Edge::Edge(Vertex *src, Vertex *dst, map<int, struct path_control_params> *path_control, float speedlimit) throw(edge_exception)
{
    try
    {
        SetSource(src);
        SetDestination(dst);
        SetPathControl(path_control);
        this->speedlimit = speedlimit;
    }
    catch(edge_exception *e)
    {
        throw;
    }
}

// Operator ==
bool Edge::operator==(Edge e)
{
    return ((*src == *e.GetSource()) && (*dst == *e.GetDestination()));
}

// Getters and setters
Vertex *Edge::GetSource(void)
{
    return this->src;
}

Vertex *Edge::GetDestination(void)
{
    return this->dst;
}

void Edge::SetSource(Vertex *src) throw(edge_exception)
{
    if (src != NULL)
        this->src = src;
    else
        throw EX_EDGE_INVALID_SRC;
}

void Edge::SetDestination(Vertex *dst) throw(edge_exception)
{
    if (dst != NULL)
        this->dst = dst;
    else
        throw EX_EDGE_INVALID_DST;
}

void Edge::SetPathControl(map<int, struct path_control_params> *path_control)
{
	this->path_control = path_control;
}

map<int, struct path_control_params> *Edge::GetPathControl(void)
{
	return this->path_control;
}

/* Return the weight of this edge
 *
 * If there is a path control, check its current speed limit and traffic
 * congestion, to return the total time to travel the road.
 *
 * If there is no path control, just return the path's length.
 **/
float Edge::GetWeight(int time)
{
	float distance, speedlimit, traffic, speed, totaltime = 0;

	// Is there any traffic control?
	if (path_control->size() > 1)
	{
		// Search for the last change in this path
		struct path_control_params params;
		map<int, struct path_control_params>::iterator it = this->path_control->lower_bound(time);

		if (it == this->path_control->end())
			it--;

		params = it->second;

		if (params.blocked)
			return std::numeric_limits<double>::max();	// Infinite
		else
		{
			distance = this->src->GetPoint().Distance(this->dst->GetPoint());
			speedlimit = params.speedlimit == 0 ? this->speedlimit : params.speedlimit;
			traffic = params.traffic;
			speed = speedlimit * traffic;
			totaltime = (speed == 0) ? std::numeric_limits<float>::max() : distance / speed;
		}
	}
	// Else, we use the default speed limit (if any) or the path's length
	else
	{
		distance = this->src->GetPoint().Distance(this->dst->GetPoint());
		speedlimit = this->speedlimit;
		traffic = 1;
		speed = speedlimit * traffic;
		totaltime = (speed == 0) ? this->GetLength() : distance / speed;
	}

	return totaltime;
}

// Return the length of the edge
float Edge::GetLength(void)
{
	return this->src->GetPoint().Distance(this->dst->GetPoint());
}

// Return the speed limit
float Edge::GetSpeedLimit(void)
{
	return this->speedlimit;
}
