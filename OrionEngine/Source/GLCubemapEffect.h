//
//  GLCubemapEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-18.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__GLCubemapEffect__
#define __Orion__GLCubemapEffect__

#include "GLBaseEffect.h"

class GLCubemapEffect : public GLBaseEffect
{
public:
    GLCubemapEffect();
    ~GLCubemapEffect();
    
    bool init();
    void prepareToRender();

private:
    GLint m_iLocCubemap;
};

#endif /* defined(__Orion__GLCubemapEffect__) */
