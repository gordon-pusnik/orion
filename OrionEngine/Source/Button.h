//
//  Button.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-22.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Button__
#define __Orion__Button__

#include "GLTextureEffect.h"
#include "GLBatch.h"

class Button
{
public:
    Button();
    ~Button();
    void init(Vector3f position, float size);
    void update(float dt);
    void render();
    bool touchesBegan(float x, float y);
    void touchesMoved(float x, float y);
    void touchesEnded(float x, float y);
    bool pressed() { return m_bPressed; };
    void setPressed(bool b) { m_bPressed = b; };
    
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
    GLuint m_uiAltTextureID;
    float m_fViewWidth;
    float m_fViewHeight;
    float m_fSize;
    bool m_bPressed;
    Matrix4 m_modelViewMatrix;
};

#endif /* defined(__Orion__Button__) */
