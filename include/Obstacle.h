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
