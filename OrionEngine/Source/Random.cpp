//
//  Random.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-30.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Random.h"
#include "Constants.h"
#include <cmath>

unsigned int Random::wangHash(unsigned int seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

unsigned int rng_state;

void Random::seedRand(unsigned int seed)
{
    rng_state = wangHash(seed);
}

unsigned int Random::randXorshift()
{
    // Xorshift algorithm from George Marsaglia's paper
    rng_state ^= (rng_state << 13);
    rng_state ^= (rng_state >> 17);
    rng_state ^= (rng_state << 5);
    return rng_state;
}

double Random::randDouble()
{
    return double(randXorshift()) * (1.0 / 4294967296.0);
}

double Random::randDouble(double max)
{
    return max * randDouble();
}

double Random::randomInRange(double min, double max)
{
    return randDouble(max - min) + min;
}



