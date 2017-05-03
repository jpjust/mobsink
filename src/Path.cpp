/*
 * Path class implementation.
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

#include "Path.h"
#include <math.h>

// Constructors
Path::Path(int flow)
{
	ResetControlParams();
	SetFlow(flow);
}

Path::Path(Point a, Point b, int flow)
{
	ResetControlParams();
    SetPointA(a);
    SetPointB(b);
	SetFlow(flow);
}

Path::Path(float xa, float ya, float xb, float yb, int flow)
{
	ResetControlParams();
    Point a(xa, ya);
    Point b(xb, yb);
    SetPointA(a);
    SetPointB(b);
	SetFlow(flow);
}

// Getters and setters
Point Path::GetPointA(void)
{
    return this->a;
}

Point Path::GetPointB(void)
{
    return this->b;
}

int Path::GetFlow(void)
{
	return this->flow;
}

void Path::SetPointA(Point a)
{
    this->a = a;
}

void Path::SetPointB(Point b)
{
    this->b = b;
}

void Path::SetFlow(int flow)
{
	this->flow = flow;
}

// Return the lenght of this path
float Path::GetLenght(void)
{
    return a.Distance(b);
}

// Return true if the Point p is inside the Path (within a 1 pixel error)
bool Path::HasPoint(Point p)
{
    if (round(p.Distance(a) + p.Distance(b)) <= round(GetLenght() + 1))
        return true;
    else
        return false;
}

// Return the projection of Point p on Path
// NOTE: Path will be treated as a line and the result Point
// must be checked if it is inside the Path after.
// Use Path::GetNearestPoint() to do this.
Point Path::GetProjection(Point p)
{
    Point r;
    float M[2][3];

    // If angle is 0º, 90º, 180º or 270º, we use a different approach
    if (a.GetX() == b.GetX())
    {
        r.SetX(a.GetX());
        r.SetY(p.GetY());
    }
    else if (a.GetY() == b.GetY())
    {
        r.SetX(p.GetX());
        r.SetY(a.GetY());
    }
    else
    {
        /* In the normal approach, we need to build the following matrix:
         *
         * | m1 -1 -n1 |
         * | m2 -1 -n2 |
         *
         * And solve it to find m1x1 + n1 = m2x2 + n2
         */

        // Get Path tan
        M[0][0] = (a.GetY() - b.GetY()) / (a.GetX() - b.GetX());

        // Get Path linear coefficient
        M[0][2] = -(a.GetY() - a.GetX() * M[0][0]);

        // Get p-line tan
        M[1][0] = -1/M[0][0];

        // Get p-line linear coefficient
        M[1][2] = -(p.GetY() - p.GetX() * M[1][0]);

        // Fill the y coefficients
        M[0][1] = M[1][1] = -1;

        // Use Gauss Elimination to solve the system
        float multiplier = M[1][0] / M[0][0];
        for (unsigned int i = 0; i < 3; i++)
            M[1][i] = M[1][i] - multiplier * M[0][i];

        // Calculate y and x
        r.SetY(M[1][2] / M[1][1]);
        r.SetX((M[0][2] - M[0][1] * r.GetY()) / M[0][0]);
    }

    return r;
}

// Return the nearest point from p, inside the path
Point Path::GetNearestPoint(Point p)
{
    // First, we get the projection of Point p
    Point r = GetProjection(p);

    // If r is inside the path, just use it
    if (HasPoint(r))
        return r;

    // Otherwise, just move r to a or b (nearest)
    if (r.Distance(a) < r.Distance(b))
        return a;
    else
        return b;
}

// Get the intersection point of this path and r. exist will be
// true if such point exists and false otherwise.
Point Path::GetIntersection(Path r, bool &exist)
{
    Point p;
    float M[2][3];

    // Get Path tan (vertical lines have infinite tan)
    if (a.GetX() == b.GetX())
        M[0][0] = (unsigned long int)(-1);
    else
        M[0][0] = (a.GetY() - b.GetY()) / (a.GetX() - b.GetX());

    // Get Path linear coefficient
    M[0][2] = -(a.GetY() - a.GetX() * M[0][0]);

    // Get r Path tan (vertical lines have infinite tan)
    if (r.GetPointA().GetX() == r.GetPointB().GetX())
        M[1][0] = (unsigned long int)(-1);
    else
        M[1][0] = (r.GetPointA().GetY() - r.GetPointB().GetY()) / (r.GetPointA().GetX() - r.GetPointB().GetX());

    // Get r Path linear coefficient
    M[1][2] = -(r.GetPointA().GetY() - r.GetPointA().GetX() * M[1][0]);

    // Fill the y coefficients
    M[0][1] = M[1][1] = -1;

    // Use Gauss Elimination to solve the system
    float multiplier = M[1][0] / M[0][0];
    for (unsigned int i = 0; i < 3; i++)
        M[1][i] = M[1][i] - multiplier * M[0][i];

    // Calculate y and x
    p.SetY(M[1][2] / M[1][1]);
    p.SetX((M[0][2] - M[0][1] * p.GetY()) / M[0][0]);

    exist = (HasPoint(p) && r.HasPoint(p));
    return p;
}

// Reset control parameters
void Path::ResetControlParams(void)
{
	InsertControl(0, 1, false);
}

// Insert control settings at a specific time
void Path::InsertControl(int time, int weight, bool blocked)
{
    struct path_control_params p;
    p.weight = weight;
    p.blocked = blocked;
    this->path_control.insert(pair<int, struct path_control_params>(time, p));
}

// Return a pointer to this path's path control
map<int, struct path_control_params> *Path::GetPathControl(void)
{
	return &(this->path_control);
}
