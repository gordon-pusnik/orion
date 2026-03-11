//
//  GLParticleEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//
//  Point sprite shader.

#ifndef __Orion__GLParticleEffect__
#define __Orion__GLParticleEffect__

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
#include "Vector3.h"
#include "Vector4.h"
#include "GLBaseEffect.h"

class GLParticleEffect : public GLBaseEffect
{
public:
    GLParticleEffect();
    ~GLParticleEffect();
    
    bool init();
    void prepareToRender();
    
    void setEmitterPosition(Vector3f pos) {
        m_vEmitterPos[0] = pos[0];
        m_vEmitterPos[1] = pos[1];
        m_vEmitterPos[2] = pos[2];
    };

    void setVelocity(Vector3f pos) {
        m_vVelocity[0] = pos[0];
        m_vVelocity[1] = pos[1];
        m_vVelocity[2] = pos[2];
    };

    void setColor1(Vector4f color) {
        m_fColor1[0] = color[0];
        m_fColor1[1] = color[1];
        m_fColor1[2] = color[2];
        m_fColor1[3] = color[3];
    }
    
    void setColor2(Vector4f color) {
        m_fColor2[0] = color[0];
        m_fColor2[1] = color[1];
        m_fColor2[2] = color[2];
        m_fColor2[3] = color[3];
    }
    
    void setTime(GLfloat f) { m_fTime = f; };
    void setType(GLfloat f) { m_fType = f; };
    
private:
    GLint m_iLocTextureUnit;

    Vector3f m_vEmitterPos;
    Vector3f m_vVelocity;
    GLfloat m_fType;
    GLfloat m_fTime;
    
    GLint m_iLocVelocity;
    GLint m_iLocType;
    GLint m_iLocEmitterPos;
    GLint m_iLocTime;
    GLint m_iLocColor1;
    GLint m_iLocColor2;
    
    Vector4f m_fColor1;
    Vector4f m_fColor2;
};

#endif /* defined(__Orion__GLParticleEffect__) */
