//
//  Imposter.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-19.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Imposter__
#define __Orion__Imposter__

#include "GLBatch.h"
#include "GLTextureEffect.h"
#include "Renderer.h"

class Imposter
{
public:
    Imposter();
    ~Imposter();
    
    void init(Vector3f position, float size, const char *texture, BlendMode mode, bool faceCamera, bool cullFace);
    void update(double dt);
    void render();
    void faceCamera(bool b) { m_bFaceCamera = b; };
    void rotate(float rotx, float roty);
    void scale(float scale);
    void setOpacity(float f) { m_fOpacity = f; };
    
private:
    Vector3f m_vPosition;
    GLBatch *m_pTriangleBatch;
    GLTextureEffect *m_pEffect;
    GLuint m_uiTextureID;
    bool m_bCullFace;
    BlendMode m_blendMode;
    bool m_bFaceCamera;
    float m_fScale;
    float m_fRotX;
    float m_fRotY;
    float m_fZPos;
    float m_fOpacity;
};

#endif /* defined(__Orion__Imposter__) */
