//
//  GLBaseEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//
//  Flat plain shader.

#ifndef __Orion__GLBaseEffect__
#define __Orion__GLBaseEffect__

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
#include "Vector4.h"

enum VertexAttrib
{
    VertexAttribVertex = 0,
    VertexAttribColor,
    VertexAttribNormal,
    VertexAttribTexCoord0,
    VertexAttribTexCoord1,
    VertexAttribTexCoord2,
    VertexAttribIndex,
    VertexAttribPointSize,
    VertexAttribVelocity,
    VertexAttribTime,
//    VertexAttribIndex
};

typedef struct Transform
{
    Matrix4 projectionMatrix;
    Matrix4 modelViewMatrix;
} Transform;

class GLBaseEffect
{
public:
    GLBaseEffect();
    virtual ~GLBaseEffect();
    
    virtual bool init();
    virtual void prepareToRender();
    virtual void setColor(Vector4f color);
    virtual void setMatrices(Matrix4 projectionMatrix, Matrix4 modelViewMatrix);
    
protected:
    Transform m_transform;
    GLfloat m_fColor[4];
    GLuint m_uiProgramObject;
    GLint m_iLocTransformP;
    GLint m_iLocTransformM;
    GLint m_iLocColor;
    GLint m_iLocNear;
    GLint m_iLocFar;
};

enum EffectType
{
    BASE_EFFECT = 0,
    CORONA_EFFECT,
    CUBEMAP_EFFECT,
    MULTI_TEXTURE_EFFECT,
    POINT_SPRITE_EFFECT,
    TEXTURE_EFFECT,
    TEXTURE_POINT_LIGHT_EFFECT,
    POINT_SPRITE_STAR_EFFECT,
    POINT_SPRITE_STAR_EFFECT2,
    POINT_SPRITE_RETICLE_EFFECT,
    POINT_SPRITE_ENGINE_EFFECT
};

#endif /* defined(__Orion__GLBaseEffect__) */
