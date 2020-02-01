#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <limits>

// TODO use modern C++ random  generation to create a number of helper functions to create "biologically" distrubuted random numbers (mutation size based on normal distribution e.g.)
// allow seeding / saving / loading of random state
// helper functions for ranges etc that do it right

// Thought this would be a good area for you to explore Tom, c++ has very nice modern classes for cool, clever, efficient, neatly wrapped random operations

template<typename NumericType>
NumericType Random(NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
{
    // VERY BAD IMPL!
    return static_cast<NumericType>(((static_cast<double>(std::rand()) / RAND_MAX) * (max - min)) + min);
}

#endif // RANDOM_H
