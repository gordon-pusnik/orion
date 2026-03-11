//
//  Matrix4.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Matrix4__
#define __Orion__Matrix4__

#include "Vector3.h"

class Matrix4
{
public:
    Matrix4();
    ~Matrix4();
    
    void translate(Vector3f v);
    void multiply(Matrix4 b);
    void rotate(float ang, float x, float y, float z);
    void scale(float x, float y, float z);
    void print();
    Matrix4 inverseOf();
    
    static Matrix4 makeIdentityMatrix();
    static Matrix4 makeFrustumMatrix(float left, float right, float bottom, float top, float nearZ, float farZ);
    static Matrix4 makePerspectiveMatrix(float fovy, float aspect, float nearZ, float farZ);
    static Matrix4 makeOrthoMatrix(float left, float right, float bottom, float top, float nearZ, float farZ);
    static Matrix4 makeRotationMatrix(float ang, float x, float y, float z);
    static Matrix4 makeRotationMatrix(const Vector3f rx, const Vector3f ry, const Vector3f rz);
    static Matrix4 makeTranslationMatrix(Vector3f v);
    static Matrix4 makeInfiniteMatrix(float focus, float near);
    static Matrix4 makePerspectiveMatrix(float fovy, float aspect, float nearZ, float farZ, bool infinite);
    static Matrix4 makePerspectiveMatrix(float left, float right, float bottom, float top, float nearval, float farval, bool infinite);
    static void multiply(Matrix4* product, const Matrix4 a, const Matrix4 b);

    // m0 = m00, m1 = m01, m2 = m02, m3 = m03;
    // m4 = m10, m5 = m11, m6 = m12, m7 = m13;
    // m8 = m20, m9 = m21, m10 = m22, m11 = m23;
    // m12 = m30, m13 = m31, m14 = m32, m15 = m33;
    
    float m[16];
};

#endif /* defined(__Orion__Matrix4__) */
