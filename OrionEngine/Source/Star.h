//
//  Star.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-25.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Star__
#define __Orion__Star__

#include "Entity.h"
#include "GLTextureEffect.h"
#include "Vector3.h"
#include "GLBatch.h"
#include "GLCoronaEffect.h"

class Star : public Entity
{
public:
    Star();
    ~Star();
    
    void init(EntityType e, Vector3f position, float radius, const char *texture, Vector4f color, float rotInc);
    void update(float dt);
    void render();
    
private:
    GLBaseEffect *m_pEffect;
    GLBatch *m_pTriangleBatch;
    GLfloat *m_pVertices;
    GLfloat *m_pNormals;
    GLfloat *m_pTexCoords;
    GLushort *m_pIndices;
    int m_iNumIndices;
    
    float m_fRotation;
    GLuint m_uiTextureID;
    float m_fRotInc;
    Vector4f m_fColor;
    
    GLCoronaEffect *m_pCoronaEffect;
    GLBatch *m_pCoronaBatch;
    GLfloat *m_pCoronaVerts;
    GLfloat *m_pCoronaColors;
    GLushort *m_pCoronaIndices;
    
    int m_iNumCoronaVerts;
    int m_iNumCoronaIndices;
    float m_fStep;
    double m_dPrevDistance;
    
    GLBatch *m_pPointBatch;
    
    Matrix4 m_modelViewMatrixBody;
    Matrix4 m_modelViewMatrixCorona;
    
    bool m_bRenderBody;
};

#endif /* defined(__Orion__Star__) */
