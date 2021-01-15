#ifndef TESTSHAPE_H
#define TESTSHAPE_H

#include "Shape.h"

#include <assert.h>
#include <vector>

// TODO see https://doc.qt.io/qt-5/qtest-overview.html
class TestShape {
public:
    void TestCollisions()
    {
        Circle circle{ 0.0, 0.0, 1.0 };
        std::vector<Line> collidingLines{
            { 0.5, 0.5, -0.5, -0.5 }, // Both points in
            { 0.5, 0.5, 10.0, -2.0 }, // One point in
            { -5.0, 0.0, 5.0, 0.0 }, // horizontal through middle
            { 0.0, -5.0, 0.0, 5.0 }, // vertical through middle
            { 0.0, 1.4, 1.4, 0.0 }, // slice through tl
            { 0.0, 1.4, -1.4, 0.0 }, // slice through tr
            { 0.0, -1.4, 1.4, 0.0 }, // slice through bl
            { 0.0, -1.4, -1.4, 0.0 }, // slice through br
        };
        std::vector<Line> nonCollidingLines{

        };

        for (auto& line : collidingLines) {
            assert(Collides(line, circle));
        }
        for (auto& line : nonCollidingLines) {
            assert(!Collides(line, circle));
        }
    }

private:

};

#endif // TESTSHAPE_H
