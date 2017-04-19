/*
 * Obstacle class for MobSink.
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

#include "Obstacle.h"

// Constructor
Obstacle::Obstacle(float x, float y, float radius)
    :Point(x, y)
{
    SetRadius(radius);
}

// Getters and setters
float Obstacle::GetRadius(void)
{
    return this->radius;
}

void Obstacle::SetRadius(float radius)
{
    // Radius cannot be less than 1
    if (radius >= 1)
        this->radius = radius;
}
