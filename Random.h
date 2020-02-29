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
    static NumericType Gaussian(NumericType mean = std::numeric_limits<NumericType>::min(), NumericType standardDeviation = static_cast<NumericType>(1.0))
    {
        static std::normal_distribution<NumericType> distribution;
        distribution.param(typename std::normal_distribution<NumericType>::param_type(mean, standardDeviation));
        return Number(distribution);
    }

    template<typename NumericType>
    static NumericType Int(NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        static std::uniform_int_distribution<NumericType> distribution;
        distribution.param(typename std::uniform_int_distribution<NumericType>::param_type(min, max));
        return Number(distribution);
    }

    template<typename NumericType>
    static NumericType Real(NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        static std::uniform_real_distribution<NumericType> distribution;
        distribution.param(typename std::uniform_real_distribution<NumericType>::param_type(min, max));
        return Number(distribution);
    }

    template<typename NumericType>
    static std::vector<NumericType> GaussianArray(typename std::vector<NumericType>::size_type count, NumericType mean = std::numeric_limits<NumericType>::min(), NumericType standardDeviation = static_cast<NumericType>(1.0))
    {
        static std::normal_distribution<NumericType> distribution;
        distribution.param(typename std::normal_distribution<NumericType>::param_type(mean, standardDeviation));

        std::vector<NumericType> rands;
        rands.reserve(count);
        std::generate_n(std::back_inserter(rands), count, [&](){ return Number(distribution); });
        return rands;
    }

    template<typename NumericType>
    static std::vector<NumericType> IntArray(typename std::vector<NumericType>::size_type count, NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        static std::uniform_int_distribution<NumericType> distribution;
        distribution.param(typename std::uniform_int_distribution<NumericType>::param_type(min, max));

        std::vector<NumericType> rands;
        rands.reserve(count);
        std::generate_n(std::back_inserter(rands), count, [&](){ return Number(distribution); });
        return rands;
    }

    template<typename NumericType>
    static std::vector<NumericType> RealArray(typename std::vector<NumericType>::size_type count, NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        static std::uniform_real_distribution<NumericType> distribution;
        distribution.param(typename std::uniform_real_distribution<NumericType>::param_type(min, max));

        std::vector<NumericType> rands;
        rands.reserve(count);
        std::generate_n(std::back_inserter(rands), count, [&](){ return Number(distribution); });
        return rands;
    }

    template<typename DistributionType>
    static typename DistributionType::result_type Number(DistributionType& distribution)
    {
        return distribution(entropy_);
    }

private:
    inline static std::mt19937 entropy_ = std::mt19937();
};

#endif // RANDOM_H
