//
//  GLPointSpriteStarEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//
//  Point sprite shader.

#ifndef __Orion__GLPointSpriteStarEffect__
#define __Orion__GLPointSpriteStarEffect__

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

class GLPointSpriteStarEffect : public GLBaseEffect
{
public:
    GLPointSpriteStarEffect();
    ~GLPointSpriteStarEffect();
    
    bool init();
    void prepareToRender();
    
private:
    GLint m_iLocTextureUnit;
};

#endif /* defined(__Orion__GLPointSpriteStarEffect__) */
