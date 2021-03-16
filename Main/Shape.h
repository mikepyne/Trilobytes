#ifndef SHAPEH
#define SHAPEH

#include "Utils.h"
#include <nlohmann/json.hpp>
#include "JsonHelpers.h"

#include <limits>
#include <math.h>
#include <stdint.h>
#include <assert.h>

struct Vec2 {
    double x;
    double y;
};

struct Point {
    double x;
    double y;
};

struct Line {
    Point a;
    Point b;
};

struct Circle {
    double x;
    double y;
    // inclusive
    double radius;
};

struct Rect {
    // inclusive, top left point is point closest to (0, 0)
    double left;
    double top;
    // exclusive
    double right;
    double bottom;
};

inline bool operator==(const Point& p1, const Point& p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

inline bool operator==(const Rect& r1, const Rect& r2)
{
    return r1.left == r2.left && r1.top == r2.top && r1.right == r2.right && r1.bottom == r2.bottom;
}

inline double GetDistanceSquare(const Point& a, const Point& b)
{
    // Keep this func as sqrt is expensive, and isn't always needed
    return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
}

inline double GetDistance(const Point& a, const Point& b)
{
    // Don't use std::hypot, naive impl is fit for purpose and faster
    return std::sqrt(GetDistanceSquare(a, b));
}

inline Rect RectFromCircle(const Circle& c)
{
    return {c.x - c.radius, c.y - c.radius, c.x + c.radius , c.y + c.radius };
}

/**
 * Returns a value between 0 & Tau, o inclusive, Tau exclusive
 * North: 0
 * East : 0.5 Pi
 * South: Pi
 * West : 1.5 Pi
 */
inline double GetBearing(const Point& from, const Point& to)
{
    double xDiff = to.x - from.x;
    double yDiff = to.y - from.y;

    return std::fmod(std::atan2(xDiff, yDiff) + Tril::Tau, Tril::Tau);
}

/**
 * Takes a bearing where 0 or Tau = North, increasing clockwise.
 */
inline Point ApplyOffset(Point start, double bearing, double distance)
{
    return { start.x + (std::sin(bearing) * distance), start.y + (std::cos(bearing) * distance) };
}

inline Vec2 GetMovementVector(double bearing, double speed)
{
    return { std::sin(bearing) * speed, std::cos(bearing) * speed };
}

inline std::tuple<double, double> DeconstructMovementVector(const Vec2& movementVector)
{
    double bearing = GetBearing({ 0, 0 }, { movementVector.x, movementVector.y });
    double speed = GetDistance({ 0, 0 }, { movementVector.x, movementVector.y });
    return { bearing, speed };
}

inline Rect BoundingRect(const Point& point, double margin = 0.0)
{
    assert(margin >= 0.0);
    double minX = point.x - margin;
    double maxX = point.x + margin;
    double minY = point.y - margin;
    double maxY = point.y + margin;
    return Rect{ minX, minY, maxX, maxY };
}

inline Rect BoundingRect(const Line& line, double margin = 0.0)
{
    assert(margin >= 0.0);
    double minX = std::min(line.a.x, line.b.x) - margin;
    double maxX = std::max(line.a.x, line.b.x) + margin;
    double minY = std::min(line.a.y, line.b.y) - margin;
    double maxY = std::max(line.a.y, line.b.y) + margin;
    return Rect{ minX, minY, maxX, maxY };
}

inline Rect BoundingRect(const Rect& rect, double margin = 0.0)
{
    assert(margin >= 0.0);
    double minX = rect.left   - margin;
    double maxX = rect.right  + margin;
    double minY = rect.bottom - margin;
    double maxY = rect.top    + margin;
    return Rect{ minX, minY, maxX, maxY };
}

inline Rect BoundingRect(const Circle& circle, double margin = 0.0)
{
    double minX = (circle.x - circle.radius) - margin;
    double maxX = (circle.x + circle.radius) + margin;
    double minY = (circle.y - circle.radius) - margin;
    double maxY = (circle.y + circle.radius) + margin;
    return Rect{ minX, minY, maxX, maxY };
}

inline bool Contains(const Line& l, const Point& p)
{
    if (p == l.a || p == l.b) {
        return true;
    } else if (Tril::Range<double>(l.a.x, l.b.x).Contains(p.x) && Tril::Range<double>(l.a.y, l.b.y).Contains(p.y)) {
        // Work out the slope of the line & a line connecting the point to the line
        double ldx = l.b.x - l.a.x;
        double ldy = l.b.y - l.a.y;
        double pdx = l.b.x - p.x;
        double pdy = l.b.y - p.y;

        // Work out the slopes of the lines (checking for infinite slopes)
        if (ldy == 0.0 || pdy == 0.0) {
            return ldy == pdy && l.a.x == p.x && Tril::Range<double>(l.a.x, l.b.x).Contains(p.x);
        }


        // Allow for floating point error
        double lSlope = ldx / ldy;
        double pSlope = pdx / pdy;
        return std::abs(lSlope - pSlope) < 0.0000001;
    }
    return false;
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

inline bool Collides(const Line& line, const Circle& circle)
{
    if (Contains(circle, line.a) || Contains(circle, line.b)) {
        return true;
    }

    double lineLength = GetDistance(line.a, line.b);

    // Dot product of line and circle
    double lineDeltaX = line.b.x - line.a.x;
    double lineDeltaY = line.b.y - line.a.y;
    double dot = (((circle.x - line.a.x) * (lineDeltaX)) + ((circle.y - line.a.y) * (lineDeltaY))) / std::pow(lineLength , 2.0);

    // Point on infinite line closest to circle
    Point nearest{ line.a.x + (dot * (lineDeltaX)), line.a.y + (dot * (lineDeltaY)) };

    // Check point is not past either end of the specified line
    if (!Contains(line, nearest)) {
        return false;
    }

  return GetDistance({ circle.x, circle.y }, nearest) <= circle.radius;
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

inline bool Collides(const Circle& c1, const Circle& c2)
{
    return (std::pow(c1.x - c2.x, 2.0) + std::pow(c1.y - c2.y, 2.0)) <= std::pow(c1.radius + c2.radius, 2.0);
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

/**
 * Generically allow Collide and Contain to be synonymous when one or more of
 * the types is a Point
 */
template <typename Shape>
inline bool Collides(const Shape& s, const Point& p)
{
    return Contains(s, p);
}

/**
 * Prevent need to specify collision functions twice. See
 * https://stackoverflow.com/questions/61485764/call-a-function-that-is-specifically-not-templated
 */
template<typename Shape1, typename Shape2>
auto Collides(Shape1 a, Shape2 b) -> decltype(::Collides(b, a))
{
    // Uses SFINAE to prevent recursive calling
    return Collides(b, a);
}

inline nlohmann::json Serialise(const Vec2& v)
{
    return {
        {"x", v.x},
        {"y", v.y},
    };
}

inline nlohmann::json Serialise(const Point& p)
{
    return {
        {"x", p.x},
        {"y", p.y},
    };
}

inline nlohmann::json Serialise(const Line& l)
{
    return {
        {"a", Serialise(l.a)},
        {"b", Serialise(l.b)},
    };
}

inline nlohmann::json Serialise(const Circle& c)
{
    return {
        {"x", c.x},
        {"y", c.y},
        {"Radius", c.radius},
    };
}

inline nlohmann::json Serialise(const Rect& r)
{
    return {
        {"Left", r.left},
        {"Top", r.top},
        {"Right", r.right},
        {"Bottom", r.bottom},
    };
}

#endif // SHAPEH
