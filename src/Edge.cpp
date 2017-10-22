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
Edge::Edge(Vertex *src, Vertex *dst, map<int, struct path_control_params> *path_control) throw(edge_exception)
{
    try
    {
        SetSource(src);
        SetDestination(dst);
        SetPathControl(path_control);
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
	if (path_control)
	{
		// Search for the last change in this path
		for (unsigned int i = time; i >= 0; i--)
		{
			if (this->path_control->find(i) == this->path_control->end())
				continue;

			if (this->path_control->at(i).blocked)
				return std::numeric_limits<double>::max();	// Infinite
			else
			{
				float distance = this->src->GetPoint().Distance(this->dst->GetPoint());
				float speedlimit = this->path_control->at(i).speedlimit;
				float traffic = this->path_control->at(i).traffic;
				float speed = speedlimit * traffic;
				float totaltime = (speed == 0) ? std::numeric_limits<float>::max() : distance / speed;
				return totaltime;
			}
		}
	}

	return this->GetLength();
}

// Return the length of the edge
float Edge::GetLength(void)
{
	return this->src->GetPoint().Distance(this->dst->GetPoint());
}
