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
