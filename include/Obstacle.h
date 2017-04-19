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

#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Point.h"

// This class represents an obstacle in a WSN
class Obstacle: public Point
{
public:
    Obstacle(float x = 0, float y = 0, float radius = 1);

    float GetRadius(void);
    void SetRadius(float radius);

private:
    float radius;
};

#endif // OBSTACLE_H
