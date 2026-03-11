//
//  Random.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-30.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Random__
#define __Orion__Random__

namespace Random
{
    unsigned int wangHash(unsigned int seed);
    void seedRand(unsigned int seed);
    unsigned int randXorshift();
    double randDouble();
    double randDouble(double max);
    double randomInRange(double min, double max);
};

#endif /* defined(__Orion__Random__) */
