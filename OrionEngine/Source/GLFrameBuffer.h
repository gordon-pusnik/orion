//
//  GLFrameBuffer.h
//  Particlizer
//
//  Created by Gordon Pusnik on 2013-02-10.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Particlizer__GLFrameBuffer__
#define __Particlizer__GLFrameBuffer__

#include "GLBatch.h"
#include "GLTextureEffect.h"

class GLFrameBuffer
{
public:
    GLFrameBuffer();
    ~GLFrameBuffer();
    
    void init(float viewWidth, float viewHeight);
    void bind();
    void unbind();
    void render();

private:
    GLBatch *m_pBatch;
    GLTextureEffect *m_pEffect;
    GLuint m_uiTextureID;
    GLuint m_uiFboID;
    GLuint m_uiDepthBufferID;
    Vector3f m_vPosition;
    float m_fViewWidth;
    float m_fViewHeight;
};

#endif /* defined(__Particlizer__GLFrameBuffer__) */
