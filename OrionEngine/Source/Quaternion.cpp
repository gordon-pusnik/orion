//
//  Quaternion.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Quaternion.h"
#include <cmath>

Quaternion::Quaternion()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
    w = 1.0;
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Quaternion::Quaternion(float angle, Vector3f axis)
{
    const float halfAng = angle * 0.5f;
    const float sinHalfAng = sin(halfAng);
    w = cos(halfAng);
    x = axis[0] * sinHalfAng;
    y = axis[1] * sinHalfAng;
    z = axis[2] * sinHalfAng;
}

Quaternion::~Quaternion()
{
    
}

void Quaternion::reset()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
    w = 1.0;    
}

void Quaternion::toEuler(Vector3f v)
{
    v[0] = (float)atan2(2 * y * w - 2 * x * z,
                       1 - 2 * pow(y, 2) - 2 * pow(z, 2));
    
    v[2] = (float)asin(2 * x * y + 2 * z * w);
    
    v[1] = (float)atan2(2 * x * w - 2 * y * z,
                       1 - 2 * pow(x, 2) - 2 * pow(z, 2));
    
    if (x * y + z * w == 0.5) {
        v[0] = (float)(2 * atan2(x, w));
        v[1] = 0;
    }
    else if (x * y + z * w == -0.5) {
        v[0] = (float)(-2 * atan2(x, w));
        v[1] = 0;
    }    
}

Matrix4 Quaternion::toMatrix4()
{
    Matrix4 m;
    
    float xx = x * x;
    float xy = x * y;
    float xz = x * z;
    float xw = x * w;
    
    float yy = y * y;
    float yz = y * z;
    float yw = y * w;
    
    float zz = z * z;
    float zw = z * w;
    
    m.m[0] = 1.0 - 2.0 * (yy + zz);
    m.m[4] =       2.0 * (xy - zw);
    m.m[8] =       2.0 * (xz + yw);
    
    m.m[1] =       2.0 * (xy + zw);
    m.m[5] = 1.0 - 2.0 * (xx + zz);
    m.m[9] =       2.0 * (yz - xw);
    
    m.m[2] =       2.0 * (xz - yw);
    m.m[6] =       2.0 * (yz + xw);
    m.m[10] = 1.0 - 2.0 * (xx + yy);
    
    m.m[12] = m.m[13] = m.m[14] = m.m[3] = m.m[7] = m.m[11] = 0.0;
    m.m[15] = 1.0;
    
    return m;
}

void Quaternion::normalize()
{
    float l = 1.0 / sqrt (w * w + x * x + y * y + z * z);
    w *= l;
    x *= l;
    y *= l;
    z *= l;
}

Quaternion Quaternion::multiply(Quaternion a, Quaternion b)
{
    Quaternion r;
    r.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    r.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    r.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    r.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    return r;    
}
