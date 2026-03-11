//
//  Vector4.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-11.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Vector4__
#define __Orion__Vector4__

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#elif WIN32
#include "stdafx.h"
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include "Matrix4.h"

typedef float Vector4f[4];
typedef double Vector4d[4];

// Class 
class Vector4fC
{
public:
    Vector4fC(float a_x = 0, float a_y = 0, float a_z = 0, float a_w = 0);
    float x;
    float y;
    float z;
    float w;
};

// Class to encapsulate a Vector4d array
class Vector4dC
{
public:
    Vector4dC(double a_x = 0, double a_y = 0, double a_z = 0, double a_w = 0);
    double x;
    double y;
    double z;
    double w;
};

namespace Vector4
{
    void transformVector4(Vector4d vOut, const Vector4d v, const Matrix4 m);
};

#endif /* defined(__Orion__Vector4__) */
