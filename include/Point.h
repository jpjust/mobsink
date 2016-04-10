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
