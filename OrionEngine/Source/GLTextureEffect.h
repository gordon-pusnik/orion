//
//  GLTextureEffect.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-14.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__GLTextureEffect__
#define __Orion__GLTextureEffect__

#include "GLBaseEffect.h"
#include "Vector3.h"
#include "Vector4.h"

class GLTextureEffect : public GLBaseEffect
{
public:
    GLTextureEffect();
    ~GLTextureEffect();
    
    bool init();
    void prepareToRender();
    
private:
    GLint m_iLocTextureUnit;
};

#endif /* defined(__Orion__GLTextureEffect__) */
