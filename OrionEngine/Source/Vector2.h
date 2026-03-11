//
//  Vector2.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-11.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Vector2__
#define __Orion__Vector2__

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#elif WIN32
#include "stdafx.h"
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

typedef float Vector2f[2];
typedef double Vector2d[2];

// Class to encapsulate a Vector2f array
class Vector2fC
{
public:
    Vector2fC(float a_x = 0, float a_y = 0);
    float x;
    float y;
};

// Class to encapsulate a Vector2d array
class Vector2dC
{
public:
    Vector2dC(double a_x = 0, double a_y = 0);
    double x;
    double y;
};

namespace Vector2
{
    void normalize(Vector2f v);
    void print(Vector2f v);
};

#endif /* defined(__Orion__Vector2__) */
