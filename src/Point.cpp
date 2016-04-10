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
