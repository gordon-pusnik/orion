//
//  Camera.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Camera__
#define __Orion__Camera__

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#elif WIN32
#include "stdafx.h"
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include "Vector3.h"
#include "Matrix4.h"
#include "Quaternion.h"

class Camera
{
public:
    Camera();
    ~Camera();
    void init(Vector3f position, float viewWidth, float viewHeight);
    void update(float dt);
    float distance(Vector3f position);
    double distance(Vector3d position);
    Matrix4 projectionMatrixPerspective() { return m_mProjectionMatrixPerspective; }
    Matrix4 projectionMatrixOrtho() { return m_mProjectionMatrixOrtho; }
    Matrix4 viewMatrix() { return m_mViewMatrix; }
    Matrix4 rotationMatrix() { return m_mRotationMatrix; };
    void move(float x, float y, float z);
    void setStart(float x, float y, float z);
    void setNewPosition(float x, float y, float z);
    void setStartZ(float x, float y, float z);
    void setNewPositionZ(float x, float y, float z);
    void projectOntoSurface(Vector3f touchPoint);
    void projectOntoSurfaceZ(Vector3f touchPoint);
    void computeIncremental();
    void getViewCoords(Vector3f v);
    void getPosition(Vector3d v);
    void resetOrientation();
    void restoreOrientation();
    void addSpeed(float f) {m_fSpeed += f;};
    void setSpeed(float f) {m_fSpeed = f;};
    float getSpeed() {return m_fSpeed;};
    
private:
    Vector3f m_vAnchorPosition;
    Vector3f m_vCurrentPosition;
    
    Quaternion m_qOrientationStart;
    Quaternion m_qOrientation;
    Quaternion m_qOrientationSaved;
    Matrix4 m_mProjectionMatrixPerspective;
    Matrix4 m_mProjectionMatrixOrtho;
    Matrix4 m_mViewMatrix;
    Matrix4 m_mRotationMatrix;
    
    Vector3d m_vPosition;
    Vector3f m_vViewCoords;
    
    Vector3d m_vSavedPosition;
    
    float m_fViewWidth;
    float m_fViewHeight;
     
    float m_fSpeed;
};

#endif /* defined(__Orion__Camera__) */
