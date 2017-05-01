/*
 * Path class declarations.
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

#ifndef PATH_H
#define PATH_H

#include "Point.h"
#include <map>

using namespace std;

// Struct to define control parameters
struct path_control_params
{
    int weight;
    bool blocked;
};

// This class represents a path for the sinks
class Path
{
public:
    Path();
    Path(Point a, Point b);
    Path(float xa, float ya, float xb, float yb);
    void Reset(void);

    Point GetPointA(void);
    Point GetPointB(void);

    void SetPointA(Point a);
    void SetPointB(Point b);

    float GetLenght(void);
    bool HasPoint(Point p);
    Point GetProjection(Point p);
    Point GetNearestPoint(Point p);
    Point GetIntersection(Path r, bool &exist);
    void InsertControl(int time, int weight, bool blocked);
    map<int, struct path_control_params> *GetPathControl(void);

private:
    void ResetControlParams(void);

    Point a;
    Point b;
    int weight;
    bool blocked;
    struct path_control_params params_init;
    map<int, struct path_control_params> path_control;    // Key: time in seconds

};

#endif // PATH_H
