#ifndef PATH_H
#define PATH_H

#include "Point.h"

// This class represents a path for the sinks
class Path
{
public:
    Path();
    Path(Point a, Point b);
    Path(float xa, float ya, float xb, float yb);

    Point GetPointA(void);
    Point GetPointB(void);

    void SetPointA(Point a);
    void SetPointB(Point b);

    float GetLenght(void);
    bool HasPoint(Point p);
    Point GetProjection(Point p);
    Point GetNearestPoint(Point p);
    Point GetIntersection(Path r, bool &exist);

private:
    Point a;
    Point b;
};

#endif // PATH_H
