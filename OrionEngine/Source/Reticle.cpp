//
//  Reticle.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-07-04.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "Reticle.h"
#include "Manager.h"
#include "Utils.h"
#include "Renderer.h"

#ifdef __APPLE__
#ifndef OPENGL_ES
#include <OpenGL/gl.h>
#endif
#endif

Reticle::Reticle()
{
    
}

Reticle::~Reticle()
{
    Manager::textureManager()->releaseTextureForFileName("Images/Point.png");
    Manager::textureManager()->releaseTextureForFileName("Images/Reticle.png");
}

void Reticle::init()
{
#ifndef OPENGL_ES
    glEnable(GL_POINT_SPRITE);
#endif
   
    m_pReticleEffect = new GLPointSpriteReticleEffect;
    m_pReticleEffect->init();
    Vector4f color = {59.0 / 255.0, 255.0 / 255.0, 124.0 / 255.0, 1.0};
    m_pReticleEffect->setColor(color);
    
    GLfloat verts[] = { 0.0, 0.0, 0.0 };
    m_pPointBatch = new GLBatch;
    m_pPointBatch->begin(GL_POINTS, 1, 0);
    m_pPointBatch->copyVertexData3f(verts);
    m_pPointBatch->end();
    
    TextureManager *textureManager = Manager::textureManager();
    Tex *tex = textureManager->getTextureForFileName("Images/Point.png");
    if (tex) {
        m_uiPointTextureID = tex->m_uiTex;
    }
    else {
        glGenTextures(1, &m_uiPointTextureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_uiPointTextureID);
        
        if (GLUtils::loadPNGTexture("Images/Point.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
            LOGI("Point texture loaded\n");
        }
        
        textureManager->setTextureForFileName("Images/Point.png", m_uiPointTextureID);
    }
    
    tex = textureManager->getTextureForFileName("Images/Reticle.png");
    if (tex) {
        m_uiReticleTextureID = tex->m_uiTex;
    }
    else {
        glGenTextures(1, &m_uiReticleTextureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_uiReticleTextureID);
        
        if (GLUtils::loadPNGTexture("Images/Reticle.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
            LOGI("Reticle texture loaded\n");
        }
        
        textureManager->setTextureForFileName("Images/Reticle.png", m_uiReticleTextureID);
    }
}

void Reticle::render(Matrix4 *modelViewMatrix, bool selected)
{
#ifndef OPENGL_ES
    glEnable(GL_PROGRAM_POINT_SIZE);
#endif
    Manager::renderer()->enableDepthTest(false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiPointTextureID);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_uiReticleTextureID);
    
    m_pReticleEffect->setSelected(selected);
    m_pReticleEffect->setMatrices(Manager::camera()->projectionMatrixPerspective(), *modelViewMatrix);
    m_pReticleEffect->prepareToRender();
    
    m_pPointBatch->renderInterleavedColor();
    
    Manager::renderer()->enableDepthTest(true);
#ifndef OPENGL_ES
    glDisable(GL_PROGRAM_POINT_SIZE);
#endif

}