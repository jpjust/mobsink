/*
 * Point class declarations.
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

#ifndef POINT_H
#define POINT_H

// This class represents a point in a coordinate system
class Point
{
public:
    Point(float x = 0, float y = 0);
    bool operator==(Point p);
    bool operator!=(Point p);

    float GetX(void);
    float GetY(void);

    void SetX(float x);
    void SetY(float y);

    float Distance(Point p);     // Return the distance between this Point and Point p

private:
    float x;
    float y;
};

#endif // POINT_H
