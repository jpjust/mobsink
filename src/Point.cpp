/*
 * Point class implementation.
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

#include "Point.h"
#include <cmath>
#include <stdlib.h>

// Constructor
Point::Point(float x, float y)
{
    SetX(x);
    SetY(y);
}

// Operator ==
bool Point::operator==(Point p)
{
    return (round(GetX()) == round(p.GetX())) && (round(GetY()) == round(p.GetY()));
}

// Operator !=
bool Point::operator!=(Point p)
{
    return (round(GetX()) != round(p.GetX())) || (round(GetY()) != round(p.GetY()));
}

// Getters
float Point::GetX(void)
{
    return this->x;
}

float Point::GetY(void)
{
    return this->y;
}

// Setters
void Point::SetX(float x)
{
    this->x = x;
}

void Point::SetY(float y)
{
    this->y = y;
}

// Return the distance between this Point and Point p
float Point::Distance(Point p)
{
    return sqrtf(powf(abs(this->x - p.GetX()), 2) + powf(abs(this->y - p.GetY()), 2));
}
