#pragma once

#include <random>

class Random
{
public:
    static void init()
    {
        s_randomEngine.seed(std::random_device()());
    }

    static float Float()
    {
        return (float)s_distribution(s_randomEngine)
            / (float)std::numeric_limits<std::uint64_t>::max();
    }

private:
    static std::mt19937 s_randomEngine;
    static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;
};
