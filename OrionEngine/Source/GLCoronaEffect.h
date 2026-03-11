//
//  GLCoronaEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//
//  Corona star shader.

#ifndef __Orion__GLCoronaEffect__
#define __Orion__GLCoronaEffect__

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

class GLCoronaEffect : public GLBaseEffect
{
public:
    GLCoronaEffect();
    ~GLCoronaEffect();
    
    bool init();
    void prepareToRender();
    void setTime(float time) { m_fTime = time; };
    
private:
    GLint m_iLocTime;
    float m_fTime;
};

#endif /* defined(__Orion__GLCoronaEffect__) */
