//
//  PitchYawController.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-22.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__PitchYawController__
#define __Orion__PitchYawController__

#include "GLTextureEffect.h"
#include "GLBatch.h"

class PitchYawController
{
public:
    PitchYawController();
    ~PitchYawController();
    void init(Vector3f position, float size);
    void update(float dt);
    void render();
    void touchesBegan(float x, float y);
    void touchesMoved(float x, float y);
    void touchesEnded(float x, float y);
    float getX() {return m_fX;};
    float getY() {return m_fY;};
    void setX(float x) {m_fX = x;};
    void setY(float y) {m_fY = y;};
    
private:    
    GLTextureEffect *m_pEffect;
    GLfloat *m_pVertices;
    GLfloat *m_pNormals;
    GLfloat *m_pTexCoords;
    GLushort *m_pIndices;
    int m_iNumIndices;
    Vector3f m_vPosition;
    GLBatch m_triangleBatch;
    GLuint m_uiTextureID;
    float m_fSize;
    float m_fX;
    float m_fY;
    Matrix4 m_modelViewMatrix;
};

#endif /* defined(__Orion__PitchYawController__) */
