//
//  Vector4.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-11.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Vector4.h"
#include "Matrix4.h"

Vector4fC::Vector4fC(float a_x, float a_y, float a_z, float a_w)
{
    x = a_x;
    y = a_y;
    z = a_z;
    w = a_w;
}

Vector4dC::Vector4dC(double a_x, double a_y, double a_z, double a_w)
{
    x = a_x;
    y = a_y;
    z = a_z;
    w = a_w;
}

// Full four component transform
void Vector4::transformVector4(Vector4d vOut, const Vector4d v, const Matrix4 m)
{
    vOut[0] = m.m[0] * v[0] + m.m[4] * v[1] + m.m[8] *  v[2] + m.m[12] * v[3];
    vOut[1] = m.m[1] * v[0] + m.m[5] * v[1] + m.m[9] *  v[2] + m.m[13] * v[3];
    vOut[2] = m.m[2] * v[0] + m.m[6] * v[1] + m.m[10] * v[2] + m.m[14] * v[3];
    vOut[3] = m.m[3] * v[0] + m.m[7] * v[1] + m.m[11] * v[2] + m.m[15] * v[3];
}
