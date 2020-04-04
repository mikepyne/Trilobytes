#ifndef SHAPEH
#define SHAPEH

#include <limits>
#include <math.h>
#include <stdint.h>
#include <assert.h>

struct Point {
    const double x;
    const double y;
};

struct Line {
    const Point a;
    const Point b;
};

struct Circle {
    const double x;
    const double y;
    // inclusive
    const double radius;
};

struct Rect {
    // inclusive, top left point is point closest to (0, 0)
    const double left;
    const double top;
    // exclusive
    const double right;
    const double bottom;
};

inline bool operator==(const Rect& r1, const Rect& r2)
{
    return r1.left == r2.left && r1.top == r2.top && r1.right == r2.right && r1.bottom == r2.bottom;
}

inline double GetDistanceSquare(const Point& a, const Point& b)
{
    return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
}

inline Rect RectFromCircle(const Circle& c)
{
    return {c.x - c.radius, c.y - c.radius, c.x + c.radius , c.y + c.radius };
}

/**
 * Returns a value between Pi and -Pi
 * North: 0.0
 * East : Pi / 2.0
 * South: -Pi or Pi
 * West : -Pi / 2.0
 * To normalise for a neural network, just divide by Pi
 */
inline double GetBearing(const Point& from, const Point& to)
{
    double xDiff = from.x - to.x;
    double yDiff = from.y - to.y;

    return std::atan2(xDiff, yDiff);
}

inline Point ApplyOffset(Point start, double bearing, double distance)
{
    return { start.x + (std::sin(bearing) * distance), start.y + (-std::cos(bearing) * distance) };
}

inline bool Contains(const Circle& c, const Point& p)
{
    return std::pow(std::abs(c.x - p.x), 2) + std::pow(std::abs(c.y - p.y), 2) <= std::pow(c.radius, 2);
}

inline bool Contains(const Circle& c, const Line& l)
{
    return Contains(c, l.a) && Contains(c, l.b);
}

inline bool Contains(const Rect& r, const Point& p)
{
    return p.x >= r.left && p.x < r.right && p.y >= r.top && p.y < r.bottom;
}

inline bool Contains(const Rect& r, const Line& l)
{
    return Contains(r, l.a) && Contains(r, l.b);
}

inline bool Contains(const Rect& container, const Rect& containee)
{
    return containee.left >= container.left && containee.left < container.right && containee.right <= container.right
            && containee.top >= container.top && containee.top < container.bottom && containee.bottom <= container.bottom;
}

inline bool Contains(const Rect& r, const Circle& c)
{
    return Contains(r, RectFromCircle(c));
}

inline bool Collides(const Line& l1, const Line& l2)
{
    auto& [x1, y1] = l1.a;
    auto& [x2, y2] = l1.b;
    auto& [x3, y3] = l2.a;
    auto& [x4, y4] = l2.b;

    // calculate the direction of the lines
    double uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
    double uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

    // if uA and uB are between 0-1, lines are colliding
    if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
        // optionally, draw a circle where the lines meet
        // double intersectionX = x1 + (uA * (x2-x1));
        // double intersectionY = y1 + (uA * (y2-y1));
        return true;
    }
    return false;
}

inline bool Collides(const Line& l, const Circle& c)
{
    // Use Heron's formula
    // Make a triangle between the line ends and the centre of the circle
    // then find the "height" of the triangle between the centre  of the circle
    // and the nearest point on the line
    double sideA = std::sqrt(GetDistanceSquare({ c.x, c.y }, l.a));
    double sideB = std::sqrt(GetDistanceSquare({ c.x, c.y }, l.b));
    double sideC = std::sqrt(GetDistanceSquare(l.a, l.b)); // the base of the triangle

    // s = halfTrianglePerimeter
    double s = (sideA + sideB + sideC) / 2.0;

    // Area = sqr(s * (s - sideA) * (s - sideB) * (s - sideC))
    // Area = 0.5 * base * height // where base is sideC
    // 0.5 * sideC * height = sqr(s * (s - sideA) * (s - sideB) * (s - sideC))
    double height = 2 * (std::sqrt(s * (s - sideA) * (s - sideB) * (s - sideC)) / sideC);
    return height <= c.radius;
}

inline bool Collides(const Line& l, const Rect& r)
{
    return Contains(r, l.a)
        || Contains(r, l.b)
        || Collides(l, { { r.top, r.left }, { r.top, r.right } })
        || Collides(l, { { r.top, r.left }, { r.bottom, r.left } })
        || Collides(l, { { r.bottom, r.right }, { r.top, r.right } })
        || Collides(l, { { r.bottom, r.right }, { r.bottom, r.left } });
}

inline bool Collides(const Circle& c, const Line& l)
{
    return Collides(l, c);
}

inline bool Collides(const Circle& c1, const Circle& c2)
{
    return (std::pow(c1.x - c2.x, 2.0) + std::pow(c1.y - c2.y, 2.0)) <= std::pow(c1.radius + c2.radius, 2.0);
}

inline bool Collides(const Rect& r, const Line& l)
{
    return Collides(l, r);
}

inline bool Collides(const Rect& r1, const Rect& r2)
{
    return r2.right >= r1.left && r2.left < r1.right && r2.bottom >= r1.top && r2.top < r1.bottom;
}

inline bool Collides(const Rect& r, const Circle& c)
{
    if (Collides(r, RectFromCircle(c))) {
        // contains/above/below
        if (Collides(r, Rect{c.x, c.y - c.radius, c.x, c.y + c.radius})) {
            return true;
        }
        // left/right
        else if (Collides(r, Rect{c.x - c.radius, c.y, c.x + c.radius, c.y})) {
            return true;
        }
        // corners
        // top
        else if (c.y <= r.top) {
            // top-left
            if (c.x < r.left && Contains(c, Point{r.left, r.top})) {
                return true;
            }
            // top-right
            else if (c.x > r.right && Contains(c, Point{r.right, r.top})) {
                return true;
            }
        }
        // bottom
        else if (c.y >= r.bottom) {
            // bottom-left
            if (c.x < r.left && Contains(c, Point{r.left, r.bottom})) {
                return true;
            }
            // bottom-right
            else if (c.x > r.right && Contains(c, Point{r.right, r.bottom})) {
                return true;
            }
        } else {
            assert(false && "Collision should have been caught!");
        }
    }
    return false;
}

#endif // SHAPEH
