//
//  Reticle.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-07-04.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Reticle__
#define __Orion__Reticle__

#include "GLPointSpriteReticleEffect.h"
#include "GLBatch.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

class Reticle
{
public:
    Reticle();
    ~Reticle();
    
    void init();
    void render(Matrix4 *modelViewMatrix, bool selected);
    
protected:
    
    GLPointSpriteReticleEffect *m_pReticleEffect;
    GLBatch *m_pPointBatch;
    GLuint m_uiPointTextureID;
    GLuint m_uiReticleTextureID;
};

#endif /* defined(__Orion__Reticle__) */
