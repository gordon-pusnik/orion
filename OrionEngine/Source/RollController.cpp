//
//  RollController.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-22.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "RollController.h"
#include "GLUtils.h"
#include "Camera.h"
#include "Utils.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Manager.h"
#include "Defines.h"

RollController::RollController()
{
    m_fX = 0.0f;
    m_fY = 0.0f;
}

RollController::~RollController()
{
    glDeleteTextures(1, &m_uiTextureID);    
}

void RollController::init(Vector3f position, float size)
{
    // Load texture.
    glGenTextures(1, &m_uiTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
    
    if (GLUtils::loadPNGTexture("Images/RollController.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("RollController texture loaded\n");
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
    
/*
    float size = viewHeight / 8;
    m_vPosition[0] = size / 1.2;
    m_vPosition[1] = viewHeight / 3.5f;
    m_vPosition[2] = -0.1f;
*/
    
//    m_fViewWidth = viewWidth;
//    m_fViewHeight = viewHeight;
    
    m_fSize = size;
    m_vPosition[0] = position[0];
    m_vPosition[1] = position[1];
    m_vPosition[2] = position[2];
    
    int numVertices;
#if INTERLEAVE_DATA
    m_iNumIndices = GLUtils::genSquare(size, &m_pVertices, &m_pIndices, &numVertices);
#else
    m_iNumIndices = GLUtils::genSquare(size, &m_pVertices, &m_pNormals, &m_pTexCoords, &m_pIndices, &numVertices);
#endif
    
#if USE_VBOS
#if INTERLEAVE_DATA
    m_triangleBatch.beginInterleaved(GL_TRIANGLES, numVertices, 1);
    m_triangleBatch.copyIndices(m_pIndices, m_iNumIndices);
    m_triangleBatch.copyInterleavedVertexData(m_pVertices);
    m_triangleBatch.endInterleaved();
    
    delete[] m_pVertices;
    delete[] m_pIndices;
#else
    m_triangleBatch.begin(GL_TRIANGLES, numVertices, 1);
    m_triangleBatch.copyIndices(m_pIndices, m_iNumIndices);
    m_triangleBatch.copyVertexData3f(m_pVertices);
    m_triangleBatch.copyNormalData3f(m_pNormals);
    m_triangleBatch.copyTexCoordData2f(m_pTexCoords, 0);
    m_triangleBatch.end();
    
    delete[] m_pVertices;
    delete[] m_pNormals;
    delete[] m_pTexCoords;
    delete[] m_pIndices;
#endif
#endif    
}

void RollController::update(float dt)
{
    Vector3f position;
    position[0] = m_vPosition[0] + m_fX;
    position[1] = m_vPosition[1] + m_fY;
    position[2] = m_vPosition[2];
    m_modelViewMatrix = Matrix4::makeTranslationMatrix(position);
}

void RollController::render()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

    m_pEffect->setMatrices(Manager::camera()->projectionMatrixOrtho(), m_modelViewMatrix);
    m_pEffect->prepareToRender();
    
#if USE_VBOS
#if INTERLEAVE_DATA
    m_triangleBatch.renderInterleaved();
#else
    m_triangleBatch.render();
#endif
#else
    glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), m_pVertices);
    glEnableVertexAttribArray(VertexAttribVertex);
    
    glVertexAttribPointer(VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), m_pNormals);
    glEnableVertexAttribArray(VertexAttribNormal);
    
    glVertexAttribPointer(VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), m_pTexCoords);
    glEnableVertexAttribArray(VertexAttribTexCoord0);
    
    glDrawElements(GL_TRIANGLES, m_iNumIndices, GL_UNSIGNED_SHORT, m_pIndices);
    
    glDisableVertexAttribArray(VertexAttribVertex);
    
    glDisableVertexAttribArray(VertexAttribNormal);
    
    glDisableVertexAttribArray(VertexAttribTexCoord0);
#endif    
}

void RollController::touchesBegan(float x, float y)
{
    float minTouchX = m_vPosition[0] - (m_fSize / 2);
    float maxTouchX = m_vPosition[0] + (m_fSize / 2);
    float minTouchY = m_vPosition[1] - (m_fSize / 2);
    float maxTouchY = m_vPosition[1] + (m_fSize / 2);
    
    if (x > minTouchX && x < maxTouchX &&
        y > minTouchY && y < maxTouchY) {
        
        double diff_x = x - m_vPosition[0];
        double diff_y = 0;//y - m_vPosition[1];
        
        //double radius = maxTouchX - m_vPosition[0] - 50;
        double radius = 15;//m_fSize / 8.0;
        
        if ((pow(diff_x, 2.0) + pow(diff_y, 2.0) - pow(radius, 2)) < 0) {
            m_fX = diff_x;
            m_fY = diff_y;
        }
    }
}

void RollController::touchesMoved(float x, float y)
{
    float minTouchX = m_vPosition[0] - (m_fSize / 2);
    float maxTouchX = m_vPosition[0] + (m_fSize / 2);
    float minTouchY = m_vPosition[1] - (m_fSize / 2);
    float maxTouchY = m_vPosition[1] + (m_fSize / 2);
    
    if (x > minTouchX && x < maxTouchX &&
        y > minTouchY && y < maxTouchY) {
        
        double diff_x = x - m_vPosition[0];
        double diff_y = 0;//y - m_vPosition[1];
        
        //double radius = maxTouchX - m_vPosition[0] - 50;
        double radius = 15;//m_fSize / 8.0;
        
        if ((pow(diff_x, 2.0) + pow(diff_y, 2.0) - pow(radius, 2)) < 0) {
            m_fX = diff_x;
            m_fY = diff_y;
        }
        else {
            float bearingRadians = atan2f(diff_x, -diff_y); // get bearing in radians
            
            // Clamp to point at bearing radians
            m_fX = (radius * sin(bearingRadians));
            m_fY = -(radius * cos(bearingRadians));
        }
    }
}

void RollController::touchesEnded(float x, float y)
{
    float minTouchX = m_vPosition[0] - (m_fSize * 2);
    float maxTouchX = m_vPosition[0] + (m_fSize * 2);
    float minTouchY = m_vPosition[1] - (m_fSize * 2);
    float maxTouchY = m_vPosition[1] + (m_fSize * 2);

    if (x > minTouchX && x < maxTouchX &&
        y > minTouchY && y < maxTouchY) {
        m_fX = m_fY = 0;
    }
}

