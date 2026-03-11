//
//  Nebula.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-21.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Nebula__
#define __Orion__Nebula__

#include "GLBatch.h"
#include "GLNebulaEffect.h"
#include "Vector3.h"
#include "Renderer.h"

class Nebula
{
public:
    Nebula();
    ~Nebula();
    
    void init(Vector3f pos, int num, float xsize, float ysize, float particleSize, Vector4f color, BlendMode mode, const char* tex);
    void update(double dt);
    void render();
    void rotate(float rotx, float roty, float rotz);
    void scale(float scale);
    void pan(float x, float y);
    void setRelativeSize(float size) { m_fRelativeSize = size; };
    
private:
    Vector3f m_vPosition;
    GLBatch *m_pPointsBatch;
    GLNebulaEffect *m_pEffect;
    GLuint m_uiTextureID;
    float m_fScale;
    float m_fRotX;
    float m_fRotY;
    float m_fRotZ;
    float m_fZPos;
    float m_fPanX;
    float m_fPanY;
    float m_fRelativeSize;
    float m_fParticleScale;
    BlendMode m_blendMode;
};

#endif /* defined(__Orion__Nebula__) */
