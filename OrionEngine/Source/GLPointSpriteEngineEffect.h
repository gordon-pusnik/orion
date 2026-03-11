//
//  GLPointSpriteEngineEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//
//  Point sprite shader.

#ifndef __Orion__GLPointSpriteEngineEffect__
#define __Orion__GLPointSpriteEngineEffect__

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
#include "GLBaseEffect.h"

class GLPointSpriteEngineEffect : public GLBaseEffect
{
public:
    GLPointSpriteEngineEffect();
    ~GLPointSpriteEngineEffect();
    
    bool init();
    void prepareToRender();
    void setPower(float p) { m_fPower = p; };
    
private:
    float m_fPower;
    GLint m_iLocPower;
    GLint m_iLocTextureUnit;
};

#endif /* defined(__Orion__GLPointSpriteEngineEffect__) */
