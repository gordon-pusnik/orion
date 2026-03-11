//
//  Skybox.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-01.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Skybox__
#define __Orion__Skybox__

#include "GLCubemapEffect.h"
#include "GLBatch.h"
#include "Vector3.h"

class Starfield;
class Imposter;
class Nebula;

class Skybox
{
public:
    Skybox();
    ~Skybox();
    
    void init(Vector3f position, int scale);
    void initTextures();
    void genStars();
    void genNebula();
    void update(float dt);
    void render();
    
private:
    GLCubemapEffect *m_pEffect;
    GLfloat *m_pVertices;
    GLfloat *m_pNormals;
    GLfloat *m_pTexCoords;
    GLushort *m_pIndices;
    Vector3f m_vPosition;
    int m_iNumIndices;
    GLBatch m_triangleBatch;
    GLuint m_uiTextureID;
    GLuint m_uiFboID;
    GLuint m_uiDepthBufferID;
    
    Starfield *m_pStarfield;
    Imposter *m_pStar;
    Nebula *m_pNebula;
    Matrix4 m_rotationMatrix;
};

#endif /* defined(__Orion__Skybox__) */
