//
//  Vector3.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Vector3.h"
#include "Utils.h"
#include <cmath>
#include <cstdio>

Vector3fC::Vector3fC(float a_x, float a_y, float a_z)
{
    x = a_x;
    y = a_y;
    z = a_z;
}

Vector3dC::Vector3dC(double a_x, double a_y, double a_z)
{
    x = a_x;
    y = a_y;
    z = a_z;
}

void Vector3::normalize(Vector3f v)
{
    float x = v[0];
    float y = v[1];
    float z = v[2];
    
    float l = 1.0f / sqrt(x * x + y * y + z * z);
    x *= l;
    y *= l;
    z *= l;
    
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

void Vector3::normalize(Vector3d v)
{
    double x = v[0];
    double y = v[1];
    double z = v[2];
    
    double l = 1.0 / sqrt(x * x + y * y + z * z);
    x *= l;
    y *= l;
    z *= l;
    
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

void Vector3::crossProduct(Vector3f result, const Vector3f u, const Vector3f v)
{
	result[0] = u[1]*v[2] - v[1]*u[2];
	result[1] = -u[0]*v[2] + v[0]*u[2];
	result[2] = u[0]*v[1] - v[0]*u[1];
}

float Vector3::dotProduct(const Vector3f u, const Vector3f v)
{
    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

void Vector3::divideScalar(Vector3f v, float scalar)
{
    float inv = 1.0 / scalar;
    v[0] *= inv;
    v[1] *= inv;
    v[2] *= inv;
}

void Vector3::subtract(Vector3f result, const Vector3f a, const Vector3f b)
{
    result[0] = a[0] - b[0];
    result[1] = a[1] - b[1];
    result[2] = a[2] - b[2];
}

void Vector3::inverseOf(Vector3f v)
{
    v[0] = -v[0];
    v[1] = -v[1];
    v[2] = -v[2];
}

float Vector3::length(Vector3f v)
{
    return sqrt (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void Vector3::print(const Vector3f v)
{
    LOGI("v[0]: %.2f, v[1]: %.2f, v[2]: %.2f\n", v[0], v[1], v[2]);
}