#ifndef RANDOM_H
#define RANDOM_H

#include "Utils.h"

#include <random>
#include <limits>
#include <vector>
#include <algorithm>

class Random {
public:
    static void Seed()
    {
        static std::random_device rand;
        entropy_.seed(rand());
    }

    static void Seed(const std::mt19937::result_type& seed)
    {
        entropy_.seed(seed);
    }

    static double Bearing()
    {
        return Random::Number(0.0, EoBE::Tau);
    }

    static bool Boolean()
    {
        return static_cast<bool>(Random::Number(0u, 1u));
    }

    template<typename NumericType>
    static NumericType Number(NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        if constexpr (std::is_integral<NumericType>::value) {
            static std::uniform_int_distribution<NumericType> distribution;
            distribution.param(typename std::uniform_int_distribution<NumericType>::param_type(min, max));
            return Generate(distribution);
        } else if constexpr (std::is_floating_point<NumericType>::value) {
            static std::uniform_real_distribution<NumericType> distribution;
            distribution.param(typename std::uniform_real_distribution<NumericType>::param_type(min, max));
            return Generate(distribution);
        } else {
            static_assert(std::is_floating_point<NumericType>::value, "Random::Number requires an integral OR floating point number type to work.");
        }
    }

    template<typename NumericType>
    static NumericType Gaussian(NumericType mean = std::numeric_limits<NumericType>::min(), NumericType standardDeviation = static_cast<NumericType>(1.0))
    {
        static std::normal_distribution<NumericType> distribution;
        distribution.param(typename std::normal_distribution<NumericType>::param_type(mean, standardDeviation));
        return Generate(distribution);
    }

    template<typename NumericType>
    static NumericType Poisson(NumericType mean = std::numeric_limits<NumericType>::min())
    {
        static std::poisson_distribution<NumericType> distribution;
        distribution.param(typename std::poisson_distribution<NumericType>::param_type(mean));
        return Generate(distribution);
    }

    template<typename NumericType>
    static std::vector<NumericType> Numbers(typename std::vector<NumericType>::size_type count, NumericType min = std::numeric_limits<NumericType>::lowest(), NumericType max = std::numeric_limits<NumericType>::max())
    {
        std::vector<NumericType> rands;
        rands.reserve(count);

        if constexpr (std::is_integral<NumericType>::value) {
            static std::uniform_int_distribution<NumericType> distribution;
            distribution.param(typename std::uniform_int_distribution<NumericType>::param_type(min, max));

            std::generate_n(std::back_inserter(rands), count, [&](){ return Generate(distribution); });
        } else if constexpr (std::is_floating_point<NumericType>::value) {
            static std::uniform_real_distribution<NumericType> distribution;
            distribution.param(typename std::uniform_real_distribution<NumericType>::param_type(min, max));

            std::generate_n(std::back_inserter(rands), count, [&](){ return Generate(distribution); });
        } else {
            static_assert(std::is_floating_point<NumericType>::value, "Random::Numbers requires an integral OR floating point number type to work.");
        }

        return rands;
    }

    template<typename NumericType>
    static std::vector<NumericType> Gaussians(typename std::vector<NumericType>::size_type count, NumericType mean = std::numeric_limits<NumericType>::min(), NumericType standardDeviation = static_cast<NumericType>(1.0))
    {
        static std::normal_distribution<NumericType> distribution;
        distribution.param(typename std::normal_distribution<NumericType>::param_type(mean, standardDeviation));

        std::vector<NumericType> rands;
        rands.reserve(count);
        std::generate_n(std::back_inserter(rands), count, [&](){ return Generate(distribution); });
        return rands;
    }

    template<typename NumericType>
    static std::vector<NumericType> Poissons(typename std::vector<NumericType>::size_type count, NumericType mean = std::numeric_limits<NumericType>::min())
    {
        static std::poisson_distribution<NumericType> distribution;
        distribution.param(typename std::poisson_distribution<NumericType>::param_type(mean));

        std::vector<NumericType> rands;
        rands.reserve(count);
        std::generate_n(std::back_inserter(rands), count, [&](){ return Generate(distribution); });
        return rands;
    }

private:
    inline static std::mt19937 entropy_ = std::mt19937();

    template<typename DistributionType>
    static typename DistributionType::result_type Generate(DistributionType& distribution)
    {
        return distribution(entropy_);
    }
};

#endif // RANDOM_H
