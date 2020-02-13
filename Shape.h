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

inline Rect RectFromCircle(const Circle& c)
{
    return {c.x - c.radius, c.y - c.radius, c.x + c.radius , c.y + c.radius };
}

inline bool Contains(const Circle& c, const Point& p)
{
    return std::pow(std::abs(c.x - p.x), 2) + std::pow(std::abs(c.y - p.y), 2) <= std::pow(c.radius, 2);
}

inline bool Collides(const Rect& r1, const Rect& r2)
{
    return r2.right >= r1.left && r2.left < r1.right && r2.bottom >= r1.top && r2.top < r1.bottom;
}

inline bool Collides(const Circle& c1, const Circle& c2)
{
    return (std::pow(c1.x - c2.x, 2.0) + std::pow(c1.y - c2.y, 2.0)) <= (std::pow(c1.radius, 2.0) + std::pow(c2.radius, 2.0));
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
        else if (c.y < r.top) {
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
        else if (c.y > r.bottom) {
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

inline bool Contains(const Rect& r, const Point& p)
{
    return p.x >= r.left && p.x < r.right && p.y >= r.top && p.y < r.bottom;
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

#endif // SHAPEH
