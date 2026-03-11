//
//  SlidePanel.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-27.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "SlidePanel.h"
#include "Vector3.h"
#include "Manager.h"
#include "GLUtils.h"
#include "Utils.h"

SlidePanel::SlidePanel()
{
    m_dTimeOpened = 0;
}

SlidePanel::~SlidePanel()
{
    glDeleteTextures(1, &m_uiTextureID);
}

void SlidePanel::init(float viewWidth, float viewHeight)
{
    // Load texture.
    glGenTextures(1, &m_uiTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
    
    if (GLUtils::loadPNGTexture("Images/SlidePanel.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("Slide panel texture loaded\n");
    }
    
    // Check if this shader has already been created, otherwise create it.
    ShaderManager *shaderManager = Manager::shaderManager();
    Shader *shader = shaderManager->getShaderForEffectType(TEXTURE_EFFECT);
    if (shader) {
        m_pEffect = (GLTextureEffect*) shader->m_pEffect;
    }
    else {
        m_pEffect = new GLTextureEffect;
        m_pEffect->init();
        shaderManager->setShaderForEffectType(TEXTURE_EFFECT, m_pEffect);
    }
    
    m_bUp = false;
    
    m_fSizeW = viewWidth / 1.4;
    m_fSizeH = m_fSizeW / 5.0;
    
    m_fViewWidth = viewWidth;
    m_fViewHeight = viewHeight;
    
    m_fPosUp = m_fSizeH / 2.0;
    m_fPosDown = -m_fSizeH / 2.0 + 17.0;
    
    m_vPosition[0] = viewWidth / 2.0;
    m_vPosition[1] = m_fPosDown;
    m_vPosition[2] = -0.1f;
    
    int numVertices;
    GLfloat *pVertices;
    GLushort *pIndices;
    int numIndices = GLUtils::genRect(m_fSizeW, m_fSizeH, &pVertices, &pIndices, &numVertices);

    m_triangleBatch.beginInterleaved(GL_TRIANGLES, numVertices, 1);
    m_triangleBatch.copyIndices(pIndices, numIndices);
    m_triangleBatch.copyInterleavedVertexData(pVertices);
    m_triangleBatch.endInterleaved();
    
    delete[] pVertices;
    delete[] pIndices;

    Vector3f pos;
    float size = m_fSizeH / 2.5;
    pos[0] = m_vPosition[0] + ((m_fSizeW / 2.0) - (size / 1.2));
    pos[1] = m_fSizeH / 2.0;
    pos[2] = -0.1f;
    
    m_mapButton.init(pos, size);
}

void SlidePanel::update(float dt)
{
    if (m_bUp) {
        m_vPosition[1] = m_fPosUp;
    }
    else {
        m_vPosition[1] = m_fPosDown;
    }
    
    Vector3f position;
    position[0] = m_vPosition[0];
    position[1] = m_vPosition[1];
    position[2] = m_vPosition[2];
    m_modelViewMatrix = Matrix4::makeTranslationMatrix(position);
    
    m_mapButton.update(dt);
    
    if (m_bUp) {
        double time = Manager::timeManager()->getElapsed();
        if (time - m_dTimeOpened > 5.0) {
            m_bUp = false;
        }
    }
}

void SlidePanel::render()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

    m_pEffect->setMatrices(Manager::camera()->projectionMatrixOrtho(), m_modelViewMatrix);
    m_pEffect->prepareToRender();
    
    m_triangleBatch.renderInterleaved();
    
    if (m_bUp) {
        m_mapButton.render();
    }
}

void SlidePanel::touchesBegan(float x, float y)
{
    float minTouchX = m_vPosition[0] - (m_fSizeW / 2);
    float maxTouchX = m_vPosition[0] + (m_fSizeW / 2);
    float minTouchY = m_vPosition[1] - (m_fSizeH / 2);
    float maxTouchY = m_vPosition[1] + (m_fSizeH / 2);
    
    if (x > minTouchX && x < maxTouchX &&
        y > minTouchY && y < maxTouchY) {
        if (! m_mapButton.touchesBegan(x, y)) {
            m_bUp = ! m_bUp;
        }
        m_dTimeOpened = Manager::timeManager()->getElapsed();
    }
    else {
        m_bUp = false;
    }
}

void SlidePanel::touchesMoved(float x, float y)
{
    
}

void SlidePanel::touchesEnded(float x, float y)
{
    if (m_mapButton.pressed()) {
        m_mapButton.touchesEnded(x, y);
        if (Manager::sceneManager()->getState() == SPACE_STATE) {
            Manager::sceneManager()->setState(MAP_INIT_STATE);
        }
        else if (Manager::sceneManager()->getState() == MAP_STATE) {
            Manager::sceneManager()->setState(SPACE_INIT_STATE);
        }
    }
}

void SlidePanel::togglePanel()
{
    m_bUp = ! m_bUp;
    m_dTimeOpened = Manager::timeManager()->getElapsed();
}

void SlidePanel::pressMapButton()
{
    m_mapButton.setPressed(true);
}

void SlidePanel::releaseMapButton()
{
    m_mapButton.setPressed(false);
    if (Manager::sceneManager()->getState() == SPACE_STATE) {
        Manager::sceneManager()->setState(MAP_INIT_STATE);
    }
    else if (Manager::sceneManager()->getState() == MAP_STATE) {
        Manager::sceneManager()->setState(SPACE_INIT_STATE);
    }
}
