//
//  Ship.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-18.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Ship__
#define __Orion__Ship__

#include "Entity.h"
#include "GLTexturePointLightDiffuseEffect.h"
#include "GLBatch.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

class Ship : public Entity
{
public:
    Ship();
    ~Ship();
    
    virtual void init(Vector3f pos, const char *model, Vector3f lightPos, Vector4f lightColor, bool ourShip, float scale);
    virtual void update(float dt);
    virtual void updateOurShip(float dt);
    virtual void updateShip(float dt);
    virtual void render();
    
protected:
    GLTexturePointLightDiffuseEffect *m_pEffect;
    GLBatch *m_pBatch;
    Vector4d m_vLightPos;
    Vector4d m_vLightEyePos;
    Vector4f m_vLightColor;
    Matrix4 m_modelViewMatrix;
    float m_fRotation;
    GLuint m_uiTextureID;
    bool m_bOurShip;
    Vector3f m_orientation;
};

#endif /* defined(__Orion__Ship__) */
