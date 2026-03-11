//
//  Vector2.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-11.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Vector2.h"
#include "Utils.h"
#include <cmath>

Vector2fC::Vector2fC(float a_x, float a_y)
{
    x = a_x;
    y = a_y;
}

Vector2dC::Vector2dC(double a_x, double a_y)
{
    x = a_x;
    y = a_y;
}

void Vector2::normalize(Vector2f v)
{
    GLfloat x = v[0];
    GLfloat y = v[1];

    GLfloat invlen = 1.0f / sqrt(x * x + y * y);
    
    v[0] = x * invlen;
    v[1] = y * invlen;
}

void Vector2::print(Vector2f v)
{
    LOGI("v[0]: %.2f, v[1]: %.2f, \n", v[0], v[1]);
}