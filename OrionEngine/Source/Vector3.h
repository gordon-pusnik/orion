//
//  Vector3.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Vector3__
#define __Orion__Vector3__

typedef float Vector3f[3];
typedef double Vector3d[3];

// Class to encapsulate a Vector3f array
class Vector3fC
{
public:
    Vector3fC(float a_x = 0, float a_y = 0, float a_z = 0);
    float x;
    float y;
    float z;
};

// Class to encapsulate a Vector3d array
class Vector3dC
{
public:
    Vector3dC(double a_x = 0, double a_y = 0, double a_z = 0);
    double x;
    double y;
    double z;
};

namespace Vector3
{
    void normalize(Vector3f v);
    void normalize(Vector3d v);
    void crossProduct(Vector3f result, const Vector3f u, const Vector3f v);
    float dotProduct(const Vector3f u, const Vector3f v);
    void divideScalar(Vector3f v, float scalar);
    void subtract(Vector3f result, const Vector3f a, const Vector3f b);
    void inverseOf(Vector3f v);
    float length(Vector3f v);
    void print(const Vector3f v);
};

#endif /* defined(__Orion__Vector3__) */
