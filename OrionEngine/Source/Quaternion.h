//
//  Quaternion.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Quaternion__
#define __Orion__Quaternion__

#include "Vector3.h"
#include "Matrix4.h"

class Quaternion
{
public:
    Quaternion();
    Quaternion(float x, float y, float z, float w);
    Quaternion(float angle, Vector3f axis);
    ~Quaternion();
    void reset();
    void toEuler(Vector3f v);
    Matrix4 toMatrix4();
    void normalize();
    
    static Quaternion multiply(Quaternion a, Quaternion b);
    
    float x, y, z, w;
};

#endif /* defined(__Orion__Quaternion__) */
