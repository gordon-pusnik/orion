//
//  Engine.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-22.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Engine__
#define __Orion__Engine__

#include "Vector3.h"
#include "Vector4.h"
#include "GLPointSpriteEngineEffect.h"
#include "GLBatch.h"

class Engine
{
public:
    Engine();
    ~Engine();
    
    void init(Vector3f pos, Vector4f color);
    void setPower(float power) { m_fPower = power; };
    void update(float dt);
    void render();
    
private:
    GLPointSpriteEngineEffect *m_pEffect;
    GLBatch *m_pPointBatch;
    float m_fPower;
    Vector3d m_vPosition;
    Vector3f m_vRelPosition;
    GLuint m_uiPointTextureID;
    Matrix4 m_pointModelViewMatrix;
};

#endif /* defined(__Orion__Engine__) */
