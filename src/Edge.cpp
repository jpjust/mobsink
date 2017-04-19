/*
 * Graph's Edge class for MobSink.
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

#include <stdlib.h>
#include "Edge.h"

// Default constructor
Edge::Edge(Vertex *src, Vertex *dst) throw(edge_exception)
{
    try
    {
        SetSource(src);
        SetDestination(dst);
    }
    catch(edge_exception e)
    {
        throw;
    }
}

// Operator ==
bool Edge::operator==(Edge e)
{
    return ((*src == *e.GetSource()) && (*dst == *e.GetDestination())) || ((*dst == *e.GetSource()) && (*src == *e.GetDestination()));
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
