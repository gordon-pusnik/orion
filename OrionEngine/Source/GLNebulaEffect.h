//
//  GLNebulaEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-19.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__GLNebulaEffect__
#define __Orion__GLNebulaEffect__

#include "GLBaseEffect.h"
#include "Vector3.h"
#include "Vector4.h"

class GLNebulaEffect : public GLBaseEffect
{
public:
    GLNebulaEffect();
    ~GLNebulaEffect();
    
    bool init();
    void prepareToRender();
    
    void setRelativeSize(GLfloat f) { m_fRelativeSize = f; };
    void setScale(GLfloat f) { m_fScale = f; };
    
private:    
    GLint m_iLocLightPos;
    GLint m_iLocNoiseScale;

    GLfloat m_fRelativeSize;
    GLfloat m_fScale;
    GLint m_iLocRelativeSize;
    GLint m_iLocScale;
    
    GLint m_iLocColor1;
    GLint m_iLocColor2;
    GLint m_iLocNoiseTexture;
};

#endif /* defined(__Orion__GLTextureEffect__) */
