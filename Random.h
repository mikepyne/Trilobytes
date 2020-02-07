#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <limits>
#include <vector>
#include <algorithm>

class Random {
public:
    static void Seed(const std::mt19937::result_type& seed)
    {
        entropy_.seed(seed);
    }

    template<typename NumericType>
    static NumericType Gaussian(NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        return Number<std::normal_distribution<NumericType>>(min, max);
    }

    template<typename NumericType>
    static NumericType Int(NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        return Number<std::uniform_int_distribution<NumericType>>(min, max);
    }

    template<typename NumericType>
    static NumericType Real(NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        return Number<std::uniform_real_distribution<NumericType>>(min, max);
    }

    template<typename NumericType>
    static std::vector<NumericType> GaussianArray(typename std::vector<NumericType>::size_type count, NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        std::vector<NumericType> rands;
        rands.reserve(count);
        std::generate_n(std::back_inserter(rands), count, [&](){ return Gaussian(min, max); });
        return rands;
    }

    template<typename NumericType>
    static std::vector<NumericType> IntArray(typename std::vector<NumericType>::size_type count, NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        std::vector<NumericType> rands;
        rands.reserve(count);
        std::generate_n(std::back_inserter(rands), count, [&](){ return Int(min, max); });
        return rands;
    }

    template<typename NumericType>
    static std::vector<NumericType> RealArray(typename std::vector<NumericType>::size_type count, NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        std::vector<NumericType> rands;
        rands.reserve(count);
        std::generate_n(std::back_inserter(rands), count, [&](){ return Real(min, max); });
        return rands;
    }

private:
    inline static std::mt19937 entropy_ = std::mt19937();

    template<typename DistributionType, typename NumericType>
    static NumericType Number(NumericType min, NumericType max)
    {
        static DistributionType dist;
        dist.param(typename DistributionType::param_type(min, max));
        return dist(entropy_);
    }
};

#endif // RANDOM_H
