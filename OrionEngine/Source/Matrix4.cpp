//
//  Matrix4.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Matrix4.h"
#include "Utils.h"
#include "GLUtils.h"
#include <cmath>
#include "Constants.h"

Matrix4::Matrix4()
{
    m[0] = m[1] = m[2] = m[3] =
    m[4] = m[5] = m[6] = m[7] =
    m[8] = m[9] = m[10] = m[11] =
    m[12] = m[13] = m[14] = m[15] = 0.0;
}

Matrix4::~Matrix4()
{
    
}

#define A(row,col)  a.m[(col<<2)+row]
#define B(row,col)  b.m[(col<<2)+row]
#define P(row,col)  product->m[(col<<2)+row]

///////////////////////////////////////////////////////////////////////////////
// Multiply two 4x4 matricies
void Matrix4::multiply(Matrix4 *product, const Matrix4 a, const Matrix4 b)
{
	for (int i = 0; i < 4; i++) {
		float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
		P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
		P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
		P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
		P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
	}
}

#undef A
#undef B
#undef P

#define A(row,col)  this->m[(col<<2)+row]
#define B(row,col)  b.m[(col<<2)+row]
#define P(row,col)  this->m[(col<<2)+row]

///////////////////////////////////////////////////////////////////////////////
// Multiply by matrix
void Matrix4::multiply(const Matrix4 b)
{
	for (int i = 0; i < 4; i++) {
		float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
		P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
		P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
		P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
		P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
	}
}

#undef A
#undef B
#undef P

Matrix4 Matrix4::makeTranslationMatrix(Vector3f v)
{
    Matrix4 m = makeIdentityMatrix();
    m.m[12] = v[0];
    m.m[13] = v[1];
    m.m[14] = v[2];
    return m;
}

void Matrix4::translate(Vector3f v)
{
    Matrix4 m = makeIdentityMatrix();
    m.m[12] = v[0];
    m.m[13] = v[1];
    m.m[14] = v[2];
    multiply(m);
}

void Matrix4::scale(float x, float y, float z)
{
    Matrix4 m;
    m.m[0] = x; m.m[1] = m.m[2] = m.m[3] = 0;
    m.m[5] = y; m.m[4] = m.m[6] = m.m[7] = 0;
    m.m[10] = z; m.m[8] = m.m[9] = m.m[11] = 0;
    m.m[12] = m.m[13] = m.m[14] = 0; m.m[15] = 1;
    multiply(m);
}

Matrix4 Matrix4::makeIdentityMatrix()
{
    Matrix4 m = Matrix4();
    m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;
    return m;
}

Matrix4 Matrix4::makeFrustumMatrix(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;
    Matrix4 frust = makeIdentityMatrix();
        
    frust.m[0] = 2.0f * nearZ / deltaX;
    frust.m[1] = frust.m[2] = frust.m[3] = 0.0f;
    
    frust.m[5] = 2.0f * nearZ / deltaY;
    frust.m[4] = frust.m[6] = frust.m[7] = 0.0f;
    
    frust.m[8] = (right + left) / deltaX;
    frust.m[9] = (top + bottom) / deltaY;
    frust.m[10] = -(nearZ + farZ) / deltaZ;
    frust.m[11] = -1.0f;
    
    frust.m[14] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[12] = frust.m[13] = frust.m[15] = 0.0f;
    
    return frust;
}

Matrix4 Matrix4::makePerspectiveMatrix(float left, float right, float bottom, float top, float nearval, float farval, bool infinite)
{
    float x, y, a, b, c, d;
    x = (2.0 * nearval) / (right - left);
    y = (2.0 * nearval) / (top - bottom);
    a = (right + left) / (right - left);
    b = (top + bottom) / (top - bottom);
    
    if (infinite) {
        // Infinite view frustum
        c = -1.0;
        d = -2.0 * nearval;
    } else {
        c = -(farval + nearval) / (farval - nearval);
        d = -(2.0 * farval * nearval) / (farval - nearval);
    }
    
    Matrix4 m;
    
    m.m[0] = x;
    m.m[1] = 0;
    m.m[2] = 0;
    m.m[3] = 0;
    m.m[4] = 0;
    m.m[5] = y;
    m.m[6] = 0;
    m.m[7] = 0;
    m.m[8] = a;
    m.m[9] = b;
    m.m[10] = c;
    m.m[11] = -1;
    m.m[12] = 0;
    m.m[13] = 0;
    m.m[14] = d;
    m.m[15] = 0;

    return m;
}

Matrix4 Matrix4::makePerspectiveMatrix(float fovy, float aspect, float nearZ, float farZ, bool infinite)
{
    GLfloat frustumW, frustumH;
    
    frustumH = tanf(fovy * kPi / 360.0) * nearZ;
    frustumW = frustumH * aspect;
    
    return makePerspectiveMatrix(-frustumW, frustumW, -frustumH, frustumH, nearZ, farZ, infinite);
}

Matrix4 Matrix4::makePerspectiveMatrix(float fovy, float aspect, float nearZ, float farZ)
{
    GLfloat frustumW, frustumH;
    
    frustumH = tanf(fovy * kPi / 360.0) * nearZ;
    frustumW = frustumH * aspect;
    
    return makeFrustumMatrix(-frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
}

Matrix4 Matrix4::makeInfiniteMatrix(float focus, float nearval)
{
    Matrix4 m;
    m.m[0] = focus;
    m.m[1] = 0;
    m.m[2] = 0;
    m.m[3] = 0;
    m.m[4] = 0;
    m.m[5] = 4 * focus / 3;
    m.m[6] = 0;
    m.m[7] = 0;
    m.m[8] = 0;
    m.m[9] = 0;
    m.m[10] = 65535.0f / 65536.0f;
    m.m[11] = 1;
    m.m[12] = 0;
    m.m[13] = 0;
    m.m[14] = -nearval;
    m.m[15] = 0;

    return m;
}

Matrix4 Matrix4::makeOrthoMatrix(float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;
    Matrix4 ortho = makeIdentityMatrix();
    
    ortho.m[0] = 2.0f / deltaX;
    ortho.m[12] = -(right + left) / deltaX;
    ortho.m[5] = 2.0f / deltaY;
    ortho.m[13] = -(top + bottom) / deltaY;
    ortho.m[10] = -2.0f / deltaZ;
    ortho.m[14] = -(nearZ + farZ) / deltaZ;
    
    return ortho;
}

void Matrix4::rotate(float angle, float x, float y, float z)
{
	double mag, s, c;
	double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
    
	s = sin(angle);
	c = cos(angle);
    
	mag = sqrt( x*x + y*y + z*z );
    
    Matrix4 m;
    
	// Identity matrix
	if (mag == 0.0) {
        m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;
        multiply(m);
		return;
	}
    
	// Rotation matrix is normalized
	x /= mag;
	y /= mag;
	z /= mag;
    
#define M(row,col)  m.m[col*4+row]
    
	xx = x * x;
	yy = y * y;
	zz = z * z;
	xy = x * y;
	yz = y * z;
	zx = z * x;
	xs = x * s;
	ys = y * s;
	zs = z * s;
	one_c = 1.0f - c;
    
	M(0,0) = (one_c * xx) + c;
	M(0,1) = (one_c * xy) - zs;
	M(0,2) = (one_c * zx) + ys;
	M(0,3) = 0.0;
    
	M(1,0) = (one_c * xy) + zs;
	M(1,1) = (one_c * yy) + c;
	M(1,2) = (one_c * yz) - xs;
	M(1,3) = 0.0;
    
	M(2,0) = (one_c * zx) - ys;
	M(2,1) = (one_c * yz) + xs;
	M(2,2) = (one_c * zz) + c;
	M(2,3) = 0.0;
    
	M(3,0) = 0.0;
	M(3,1) = 0.0;
	M(3,2) = 0.0;
	M(3,3) = 1.0;
    
#undef M
    
    multiply(m);
}

Matrix4 Matrix4::makeRotationMatrix(float ang, float x, float y, float z)
{
    Matrix4 m;
	double mag, s, c;
	double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;
    
	s = sin(ang);
	c = cos(ang);
    
	mag = sqrt( x*x + y*y + z*z );
    
	// Identity matrix
	if (mag == 0.0) {
        m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;
		return m;
	}
    
	// Rotation matrix is normalized
	x /= mag;
	y /= mag;
	z /= mag;
    
#define M(row,col)  m.m[col*4+row]
    
	xx = x * x;
	yy = y * y;
	zz = z * z;
	xy = x * y;
	yz = y * z;
	zx = z * x;
	xs = x * s;
	ys = y * s;
	zs = z * s;
	one_c = 1.0f - c;
    
	M(0,0) = (one_c * xx) + c;
	M(0,1) = (one_c * xy) - zs;
	M(0,2) = (one_c * zx) + ys;
	M(0,3) = 0.0;
    
	M(1,0) = (one_c * xy) + zs;
	M(1,1) = (one_c * yy) + c;
	M(1,2) = (one_c * yz) - xs;
	M(1,3) = 0.0;
    
	M(2,0) = (one_c * zx) - ys;
	M(2,1) = (one_c * yz) + xs;
	M(2,2) = (one_c * zz) + c;
	M(2,3) = 0.0;
    
	M(3,0) = 0.0;
	M(3,1) = 0.0;
	M(3,2) = 0.0;
	M(3,3) = 1.0;
    
#undef M

    return m;
}

Matrix4 Matrix4::makeRotationMatrix(const Vector3f rx, const Vector3f ry, const Vector3f rz)
{
    Matrix4 m;
    m.m[0] = rx[0]; m.m[4] = rx[1]; m.m[8] = rx[2]; m.m[12] = 0;
    m.m[1] = ry[0]; m.m[5] = ry[1]; m.m[9] = ry[2]; m.m[13] = 0;
    m.m[2] = rz[0]; m.m[6] = rz[1]; m.m[10] = rz[2]; m.m[14] = 0;
    m.m[3] = 0; m.m[7] = 0; m.m[11] = 0; m.m[15] = 1;
    return m;
}

Matrix4 Matrix4::inverseOf()
{
    Matrix4 m;
    // this only works for matrices containing only rotation and transform
    m.m[0] = this->m[0]; m.m[1] = this->m[4]; m.m[2] = this->m[8];
    m.m[4] = this->m[1]; m.m[5] = this->m[5]; m.m[6] = this->m[9];
    m.m[8] = this->m[2]; m.m[9] = this->m[6]; m.m[10] = this->m[10];
    m.m[12] = -(this->m[0]*this->m[12] + this->m[1]*this->m[13] + this->m[2]*this->m[14]);
    m.m[13] = -(this->m[4]*this->m[12] + this->m[5]*this->m[13] + this->m[6]*this->m[14]);
    m.m[14] = -(this->m[8]*this->m[12] + this->m[9]*this->m[13] + this->m[10]*this->m[14]);
    m.m[3] = m.m[7] = m.m[11] = 0;
    m.m[15] = 1.0f;
    
    return m;
}

void Matrix4::print() {
    for (int i = 0; i < 4; i++) {
        LOGI("%.2f %.2f %.2f %.2f\n", m[i], m[i + 4], m[i + 8], m[i + 12]);
    }
}
