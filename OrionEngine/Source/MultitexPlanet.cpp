//
//  MultitexPlanet.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-01.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "MultitexPlanet.h"
#include "GLUtils.h"
#include "Utils.h"
#include "Constants.h"
#include "Defines.h"
#include "Manager.h"

MultitexPlanet::MultitexPlanet() : Entity()
{
}

MultitexPlanet::~MultitexPlanet()
{
    Manager::geometryManager()->releaseGeometryForEntityType(m_type);
    glDeleteTextures(1, &m_uiTextureID1);
    glDeleteTextures(1, &m_uiTextureID2);
    glDeleteTextures(1, &m_uiTextureID3);
}

void MultitexPlanet::init(EntityType e, Vector3f position, float radius, const char *tex1, const char *tex2, const char *tex3, Vector3f lightPos, Vector4f lightColor, float rotStart, float rotInc)
{
    m_type = e;
    
    // Load texday.
    glGenTextures(1, &m_uiTextureID1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID1);
    
    if (GLUtils::loadPNGTexture(tex1, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("MultitexPlanet - day texture loaded\n");
    }

    // Load texday.
    glGenTextures(1, &m_uiTextureID2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID2);
    
    if (GLUtils::loadPNGTexture(tex2, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("MultitexPlanet - night texture loaded\n");
    }
    
    // Load texday.
    glGenTextures(1, &m_uiTextureID3);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID3);
    
    if (GLUtils::loadPNGTexture(tex3, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("MultitexPlanet - cloud texture loaded\n");
    }    
    
    m_vLightPos[0] = lightPos[0];
    m_vLightPos[1] = lightPos[1];
    m_vLightPos[2] = lightPos[2];
    
    m_vLightColor[0] = lightColor[0];
    m_vLightColor[1] = lightColor[1];
    m_vLightColor[2] = lightColor[2];
    m_vLightColor[3] = lightColor[3];
    
    m_vPosition[0] = position[0];
    m_vPosition[1] = position[1];
    m_vPosition[2] = position[2];
    
    m_fRotation = rotStart;
    m_fRotInc = rotInc;
    
    m_fBoundingRadius = radius;
    
    // Check if this shader has already been created, otherwise create it.
    ShaderManager *shaderManager = Manager::shaderManager();
    
    Shader *shader1 = shaderManager->getShaderForEffectType(MULTI_TEXTURE_EFFECT);
    if (shader1) {
        m_pEffect = (GLMultiTexturePlanetEffect*) shader1->m_pEffect;
    }
    else {
        m_pEffect = new GLMultiTexturePlanetEffect;
        m_pEffect->init();
        shaderManager->setShaderForEffectType(MULTI_TEXTURE_EFFECT, m_pEffect);
    }
    
    Shader *shader2 = shaderManager->getShaderForEffectType(BASE_EFFECT);
    if (shader2) {
        m_pPointEffect = shader2->m_pEffect;
    }
    else {
        m_pPointEffect = new GLBaseEffect;
        m_pPointEffect->init();
        shaderManager->setShaderForEffectType(BASE_EFFECT, m_pPointEffect);
    }

    // Check if this same geometry has already been created, otherwise create it.
/*
    GeometryManager *geoManager = Manager::geometryManager();
    Geometry* geometry = geoManager->getGeometryForEntityType(m_type);
    if (geometry) {
        m_pTriangleBatch = geometry->m_pBatch1;
        m_pPointBatch = geometry->m_pBatch2;
        return;
    }
    else {
*/
        m_pTriangleBatch = new GLBatch;
        m_pPointBatch = new GLBatch;
//        geoManager->setBatchForEntityType(m_type, m_pTriangleBatch, m_pPointBatch, 0, 0, 0, 0);
//    }
    
    int numVertices;
#if INTERLEAVE_DATA
    m_iNumIndices = GLUtils::genSphere(128, radius, &m_pVertices, &m_pIndices, &numVertices);
#else
    m_iNumIndices = GLUtils::genSphere(128, radius, &m_pVertices, &m_pNormals, &m_pTexCoords, &m_pIndices, &numVertices);
#endif

#if USE_VBOS
#if INTERLEAVE_DATA
    m_pTriangleBatch->beginInterleaved(GL_TRIANGLES, numVertices, 1);
    m_pTriangleBatch->copyIndices(m_pIndices, m_iNumIndices);
    m_pTriangleBatch->copyInterleavedVertexData(m_pVertices);
    m_pTriangleBatch->endInterleaved();
    
    delete[] m_pVertices;
    delete[] m_pIndices;
#else
    m_pTriangleBatch->begin(GL_TRIANGLES, numVertices, 1);
    m_pTriangleBatch->copyIndices(m_pIndices, m_iNumIndices);
    m_pTriangleBatch->copyVertexData3f(m_pVertices);
    m_pTriangleBatch->copyNormalData3f(m_pNormals);
    m_pTriangleBatch->copyTexCoordData2f(m_pTexCoords, 0);
    m_pTriangleBatch->end();
    
    delete[] m_pVertices;
    delete[] m_pNormals;
    delete[] m_pTexCoords;
    delete[] m_pIndices;
#endif
#endif
    
    m_pPointBatch->begin(GL_POINTS, 1, 0);
//    m_pPointBatch->copyIndices(0, 1);
    Vector3f vert = { 0.0, 0.0, 0.0 };
    m_pPointBatch->copyVertexData3f(vert);
    m_pPointBatch->end();
}

void MultitexPlanet::update(float dt)
{
    m_fRotation += m_fRotInc * dt;
    if (m_fRotation >= k2Pi) {
        m_fRotation = 0.0f;
    }

    Camera *camera = Manager::camera();

    double distance = camera->distance(m_vPosition);
    double screenrad = 500.0 * m_fBoundingRadius / distance;
    
//    LOGI("Earth distance: %f, screenrad: %f\n", distance, screenrad);

    if (screenrad < 1.5) {
        m_bRenderFar = true;
    }
    else {
        m_bRenderFar = false;
    }
    
    m_modelViewMatrix = camera->viewMatrix();
        
    Vector4::transformVector4(m_vLightEyePos, m_vLightPos, m_modelViewMatrix);

    Vector3d campos;
    camera->getPosition(campos);
    
    m_vRelPosition[0] = m_vPosition[0] + campos[0];
    m_vRelPosition[1] = m_vPosition[1] + campos[1];
    m_vRelPosition[2] = m_vPosition[2] + campos[2];
    
    m_modelViewMatrix.translate(m_vRelPosition);
    m_modelViewMatrix.rotate(m_fRotation, 0.0f, 1.0f, 0.0f);
}

void MultitexPlanet::render()
{
    if (m_bRenderFar) {
        m_pPointEffect->setColor(m_vLightColor);
        m_pPointEffect->setMatrices(Manager::camera()->projectionMatrixPerspective(), m_modelViewMatrix);
        m_pPointEffect->prepareToRender();
        m_pPointBatch->render();        
    }
    else {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_uiTextureID1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_uiTextureID2);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_uiTextureID3);
            
        m_pEffect->setLightColor(m_vLightColor);
        Vector4f lightPos;
        lightPos[0] = m_vLightEyePos[0];
        lightPos[1] = m_vLightEyePos[1];
        lightPos[2] = m_vLightEyePos[2];
        m_pEffect->setLightPos(lightPos);
        m_pEffect->setMatrices(Manager::camera()->projectionMatrixPerspective(), m_modelViewMatrix);
        m_pEffect->prepareToRender();

#if USE_VBOS
#if INTERLEAVE_DATA
        m_pTriangleBatch->renderInterleaved();
#else
        m_pTriangleBatch->render();
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
}

bool MultitexPlanet::isFar()
{
    return false;
}
