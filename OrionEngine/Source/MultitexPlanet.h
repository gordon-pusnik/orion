//
//  MultitexPlanet.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-01.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__MultitexPlanet__
#define __Orion__MultitexPlanet__

#include "Entity.h"
#include "GLMultiTexturePlanetEffect.h"
#include "Vector3.h"
#include "Vector4.h"
#include "GLBatch.h"

class MultitexPlanet : public Entity
{
public:
    MultitexPlanet();
    ~MultitexPlanet();
    
    void init(EntityType e, Vector3f position, float radius, const char *tex1, const char *tex2, const char *tex3, Vector3f lightPos, Vector4f lightColor, float rotStart, float rotInc);
    void update(float dt);
    void render();
    bool isFar();
    
private:
    GLMultiTexturePlanetEffect *m_pEffect;
    GLfloat *m_pVertices;
    GLfloat *m_pNormals;
    GLfloat *m_pTexCoords;
    GLushort *m_pIndices;
    int m_iNumIndices;
    float m_fRotation;
    GLBatch *m_pTriangleBatch;
    GLuint m_uiTextureID1;
    GLuint m_uiTextureID2;
    GLuint m_uiTextureID3;
    float m_fRotInc;
    Vector4d m_vLightPos;
    Vector4d m_vLightEyePos;
    Vector4f m_vLightColor;
    Matrix4 m_modelViewMatrix;
    GLBatch *m_pPointBatch;
    GLBaseEffect *m_pPointEffect;
};

#endif /* defined(__Orion__MultitexPlanet__) */
