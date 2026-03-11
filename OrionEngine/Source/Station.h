//
//  Station.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-18.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Station__
#define __Orion__Station__

#include "Entity.h"
#include "GLTexturePointLightDiffuseEffect.h"
#include "GLBatch.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

class Station : public Entity
{
public:
    Station();
    ~Station();
    
    virtual void init(Vector3f pos, const char *model, Vector3f lightPos, Vector4f lightColor, float scale);
    virtual void update(float dt);
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
    Vector3f orientation;
};

#endif /* defined(__Orion__Station__) */
