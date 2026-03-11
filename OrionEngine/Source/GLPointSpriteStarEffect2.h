//
//  GLPointSpriteStarEffect2.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//
//  Point sprite shader.

#ifndef __Orion__GLPointSpriteStarEffect2__
#define __Orion__GLPointSpriteStarEffect2__

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

class GLPointSpriteStarEffect2 : public GLBaseEffect
{
public:
    GLPointSpriteStarEffect2();
    ~GLPointSpriteStarEffect2();
    
    bool init();
    void prepareToRender();
    
    void setRelativeSize(GLfloat f) { m_fRelativeSize = f; };
    void setScale(GLfloat f) { m_fScale = f; };
    void setSelected(float index) { m_fSelectedIndex = index; };
    float getSelected() { return m_fSelectedIndex; };
private:
    GLint m_iLocTextureUnit;
    GLfloat m_fRelativeSize;
    GLfloat m_fScale;
    float m_fSelectedIndex;
    GLint m_iLocRelativeSize;
    GLint m_iLocScale;
    GLint m_iLocSelectedIndex;
};

#endif /* defined(__Orion__GLPointSpriteStarEffect2__) */
