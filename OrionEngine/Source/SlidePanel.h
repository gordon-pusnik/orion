//
//  SlidePanel.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-27.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__SlidePanel__
#define __Orion__SlidePanel__

#include "GLTextureEffect.h"
#include "GLBatch.h"
#include "Button.h"

class SlidePanel
{
public:
    SlidePanel();
    ~SlidePanel();

    void init(float viewWidth, float viewHeight);
    void update(float dt);
    void render();
    void touchesBegan(float x, float y);
    void touchesMoved(float x, float y);
    void touchesEnded(float x, float y);
    
    void togglePanel();
    void pressMapButton();
    void releaseMapButton();
    
    bool mapButtonPressed() { return m_mapButton.pressed(); };
    
private:
    GLBatch m_triangleBatch;
    GLTextureEffect *m_pEffect;
    Vector3f m_vPosition;
    GLuint m_uiTextureID;
    
    float m_fX;
    float m_fY;
    float m_fViewWidth;
    float m_fViewHeight;
    float m_fPosUp;
    float m_fPosDown;
    float m_bUp;
    float m_fSizeW;
    float m_fSizeH;
    
    Button m_mapButton;
    
    double m_dTimeOpened;
    
    Matrix4 m_modelViewMatrix;
};

#endif /* defined(__Orion__SlidePanel__) */
