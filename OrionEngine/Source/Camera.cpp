//
//  Camera.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Camera.h"
#include "GLUtils.h"
#include "Utils.h"
#include <cmath>
#include "Constants.h"
#include "Manager.h"

Camera::Camera()
{
    m_fSpeed = 0.0f;
}

Camera::~Camera()
{
}

void Camera::resetOrientation()
{
    m_vSavedPosition[0] = m_vPosition[0];
    m_vSavedPosition[1] = m_vPosition[1];
    m_vSavedPosition[2] = m_vPosition[2];
    m_qOrientationSaved = m_qOrientation;
    m_qOrientation.reset();
}

void Camera::restoreOrientation()
{
    m_qOrientation = m_qOrientationSaved;

    m_vPosition[0] = m_vSavedPosition[0];
    m_vPosition[1] = m_vSavedPosition[1];
    m_vPosition[2] = m_vSavedPosition[2];
}

void Camera::init(Vector3f position, float viewWidth, float viewHeight)
{
    m_vPosition[0] = position[0];
    m_vPosition[1] = position[1];
    m_vPosition[2] = position[2];

    m_vSavedPosition[0] = position[0];
    m_vSavedPosition[1] = position[1];
    m_vSavedPosition[2] = position[2];

    m_qOrientation.x = 0.0f;
    m_qOrientation.y = 0.0f;
    m_qOrientation.z = 0.0f;
    m_qOrientation.w = 1.0f;

    m_qOrientationSaved.x = 0.0f;
    m_qOrientationSaved.y = 0.0f;
    m_qOrientationSaved.z = 0.0f;
    m_qOrientationSaved.w = 1.0f;

    m_qOrientationStart.x = 0.0f;
    m_qOrientationStart.y = 0.0f;
    m_qOrientationStart.z = 0.0f;
    m_qOrientationStart.w = 1.0f;
    
    float aspect = fabsf(viewWidth / viewHeight);
    LOGI("Camera::init - %f, %f, %f\n", viewWidth, viewHeight, aspect);

    m_mProjectionMatrixPerspective = Matrix4::makePerspectiveMatrix(65.0f, aspect, kNear, kFar);
    //m_mProjectionMatrixPerspective = Matrix4::makeInfiniteMatrix(2, kNear);
    //m_mProjectionMatrixPerspective = Matrix4::makePerspectiveMatrix(65.0f, aspect, kNear, kFar, true);
    m_mProjectionMatrixOrtho = Matrix4::makeOrthoMatrix(0.0f, viewWidth, 0.0f, viewHeight, 1.0f, -1.0f);

    m_mRotationMatrix = m_qOrientation.toMatrix4();
    m_mViewMatrix = m_qOrientation.toMatrix4();
}

void Camera::update(float dt)
{
    Vector3f vec;
    m_qOrientation.toEuler(vec);

    float yaw = vec[0] * (180 / kPi);
    float headingx = yaw;
    if (headingx < 0) {
        headingx = 360 + headingx;
    }
    
    float pitch = vec[1] * (180 / kPi);
    float headingy = pitch;
    if (headingy < 0) {
        headingy = 360 + headingy;
    }
    
//    LOGI("Headingx: %f, headingy: %f\n", headingx, headingy);

    m_mRotationMatrix = m_qOrientation.toMatrix4();
    m_mViewMatrix = m_qOrientation.toMatrix4();
    
    Vector3d forward;
    forward[0] = m_mViewMatrix.m[2];
    forward[1] = m_mViewMatrix.m[6];
    forward[2] = m_mViewMatrix.m[10];
    Vector3::normalize(forward);
    
    m_vViewCoords[0] = forward[0];
    m_vViewCoords[1] = forward[1];
    m_vViewCoords[2] = forward[2];

    m_vPosition[0] += forward[0] * (m_fSpeed * dt);
    m_vPosition[1] += forward[1] * (m_fSpeed * dt);
    m_vPosition[2] += forward[2] * (m_fSpeed * dt);

    //m_mViewMatrix.translate(m_vPosition);
}

void Camera::move(float x, float y, float z)
{
    if (z < 0.0f || z > 0.0f) {
        setStartZ(0.0f, 0.0f, 0.0f);
        setNewPositionZ(z, y, x);
    }
    else {
        setStart(0.0f, 0.0f, 0.0f);
        setNewPosition(x, y, z);
    }
}

void Camera::setStart(float x, float y, float z)
{
    m_vAnchorPosition[0] = x;
    m_vAnchorPosition[1] = y;
    m_vAnchorPosition[2] = z;

    projectOntoSurface(m_vAnchorPosition);
    
    m_vCurrentPosition[0] = m_vAnchorPosition[0];
    m_vCurrentPosition[1] = m_vAnchorPosition[1];
    m_vCurrentPosition[2] = m_vAnchorPosition[2];
    
    m_qOrientationStart = m_qOrientation;
}

void Camera::setNewPosition(float x, float y, float z)
{
    m_vCurrentPosition[0] = x;
    m_vCurrentPosition[1] = y;
    m_vCurrentPosition[2] = z;
    
    projectOntoSurface(m_vCurrentPosition);
    
    computeIncremental();
}

void Camera::setStartZ(float x, float y, float z)
{
    m_vAnchorPosition[0] = x;
    m_vAnchorPosition[1] = y;
    m_vAnchorPosition[2] = z;
    
    projectOntoSurfaceZ(m_vAnchorPosition);
    
    m_vCurrentPosition[0] = m_vAnchorPosition[0];
    m_vCurrentPosition[1] = m_vAnchorPosition[1];
    m_vCurrentPosition[2] = m_vAnchorPosition[2];
    
    m_qOrientationStart = m_qOrientation;
}

void Camera::setNewPositionZ(float x, float y, float z)
{
    m_vCurrentPosition[0] = x;
    m_vCurrentPosition[1] = y;
    m_vCurrentPosition[2] = z;
    
    projectOntoSurfaceZ(m_vCurrentPosition);
    
    computeIncremental();
}

void Camera::projectOntoSurface(Vector3f touchPoint)
{
    double radius = 30.0 / 3.0;
    Vector3f center = {0.0f, 0.0f, 0.0f};

    Vector3f P;
    Vector3::subtract(P, touchPoint, center);
    
    // Flip the y-axis because pixel coords increase toward the bottom.
    P[1] *= -1.0f;
    
    double radius2 = radius * radius;
    double length2 = P[0] * P[0] + P[1] * P[1];
    
    P[2] = sqrt(radius2 - length2);
    
    Vector3::normalize(P);
    
    touchPoint[0] = P[0];
    touchPoint[1] = P[1];
    touchPoint[2] = P[2];
}

void Camera::projectOntoSurfaceZ(Vector3f touchPoint)
{
    double radius = 30.0 / 3.0;
    Vector3f center = {0.0f, 0.0f, 0.0f};
    
    Vector3f P;
    Vector3::subtract(P, touchPoint, center);
    
    // Flip the y-axis because pixel coords increase toward the bottom.
    P[1] *= -1.0f;
    
    double radius2 = radius * radius;
    double length2 = P[0] * P[0] + P[2] * P[2];
    
    P[1] = sqrt(radius2 - length2);
    
    Vector3::normalize(P);
    
    touchPoint[0] = P[0];
    touchPoint[1] = P[1];
    touchPoint[2] = P[2];
}

void Camera::computeIncremental()
{
    Vector3f axis;
    Vector3::crossProduct(axis, m_vAnchorPosition, m_vCurrentPosition);
    
    double dot = Vector3::dotProduct(m_vAnchorPosition, m_vCurrentPosition);
    double angle = acosf(dot);
    
    Quaternion Q_rot(angle * 2.0f, axis);
    Q_rot.normalize();
    
    m_qOrientation = Quaternion::multiply(Q_rot, m_qOrientationStart);
}

float Camera::distance(Vector3f position)
{
    double x_diff = m_vPosition[0] + position[0];
    double y_diff = m_vPosition[1] + position[1];
    double z_diff = m_vPosition[2] + position[2];
    
    return sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff);
}

double Camera::distance(Vector3d position)
{
    double x_diff = m_vPosition[0] + position[0];
    double y_diff = m_vPosition[1] + position[1];
    double z_diff = m_vPosition[2] + position[2];

    return sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff);
}

void Camera::getViewCoords(Vector3f v)
{
    v[0] = m_vViewCoords[0];
    v[1] = m_vViewCoords[1];
    v[2] = m_vViewCoords[2];
}

void Camera::getPosition(Vector3d v)
{
    v[0] = m_vPosition[0];
    v[1] = m_vPosition[1];
    v[2] = m_vPosition[2];
}
