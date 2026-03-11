//
//  Starfield.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-12-27.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Starfield__
#define __Orion__Starfield__

#include "GLBatch.h"
#include "GLPointSpriteStarEffect.h"
#include "Vector3.h"

class Starfield
{
public:
    Starfield();
    ~Starfield();
    
    void init();
    void update(double dt);
    void render();
    
private:
    Vector3f m_vPosition;
    GLBatch *m_pPointsBatch;
    GLPointSpriteStarEffect *m_pEffect;
    GLuint m_uiTextureID;
};

#endif /* defined(__Orion__Starfield__) */
