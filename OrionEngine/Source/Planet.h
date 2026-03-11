//
//  Planet.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-01.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Planet__
#define __Orion__Planet__

#include "Entity.h"
#include "GLTexturePointLightDiffuseEffect.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GLBatch.h"
#include "GLTextureEffect.h"

class Planet : public Entity
{
public:
    Planet();
    ~Planet();
    
    void init(EntityType e, Vector3f position, float radius, const char *texture, Vector3f lightPos, Vector4f lightColor, float rotStart, float rotInc);
    void update(float dt);
    void render();
    bool isFar();
    
private:
    GLTexturePointLightDiffuseEffect *m_pEffect;
    GLfloat *m_pVertices;
    GLfloat *m_pNormals;
    GLfloat *m_pTexCoords;
    GLushort *m_pIndices;
    int m_iNumIndices;
    float m_fRotation;
    GLBatch *m_pTriangleBatch;
    GLuint m_uiTextureID;
    float m_fRotInc;
    Vector4d m_vLightPos;
    Vector4d m_vLightEyePos;
    Vector4f m_vLightColor;
    Matrix4 m_modelViewMatrix;
    GLBatch *m_pPointBatch;
    GLBaseEffect *m_pPointEffect;

    // Clouds
    GLTexturePointLightDiffuseEffect *m_pCloudEffect;
    GLuint m_uiCloudTextureID;
    GLBatch *m_pCloudTriangleBatch;
    
    // Atmosphere
    GLTexturePointLightDiffuseEffect *m_pAtmosphereEffect;
    GLuint m_uiAtmosphereTextureID;
    GLBatch *m_pAtmosphereTriangleBatch;
};

#endif /* defined(__Orion__Planet__) */
