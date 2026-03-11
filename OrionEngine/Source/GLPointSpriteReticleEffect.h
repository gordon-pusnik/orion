//
//  GLPointSpriteReticleEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//
//  Point sprite shader.

#ifndef __Orion__GLPointSpriteReticleEffect__
#define __Orion__GLPointSpriteReticleEffect__

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

class GLPointSpriteReticleEffect : public GLBaseEffect
{
public:
    GLPointSpriteReticleEffect();
    ~GLPointSpriteReticleEffect();
    
    bool init();
    void prepareToRender();
    void setSelected(bool b) { m_bSelected = b; };
    
private:
        bool m_bSelected;
    GLint m_iLocSelected;
    GLint m_iLocTextureUnit1;
    GLint m_iLocTextureUnit2;
};

#endif /* defined(__Orion__GLPointSpriteReticleEffect__) */
