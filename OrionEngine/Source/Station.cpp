//
//  Station.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-18.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "Station.h"
#include "ObjLoader.h"
#include "Manager.h"
#include "GLUtils.h"
#include "Utils.h"
#include "Constants.h"

Station::Station() : Entity()
{
    orientation[0] = 0.0;
    orientation[1] = 0.0;
    orientation[2] = 0.0;
}

Station::~Station()
{
    Manager::shaderManager()->releaseShaderForEffectType(TEXTURE_POINT_LIGHT_EFFECT);
    Manager::geometryManager()->releaseGeometryForEntityType(m_type);
}

void Station::init(Vector3f pos, const char *model, Vector3f lightPos, Vector4f lightColor, float scale)
{
    m_bSelected = false;
    
    m_fRotation = 0.0;

    m_vLightPos[0] = lightPos[0];
    m_vLightPos[1] = lightPos[1];
    m_vLightPos[2] = lightPos[2];
    
    m_vLightColor[0] = lightColor[0];
    m_vLightColor[1] = lightColor[1];
    m_vLightColor[2] = lightColor[2];
    m_vLightColor[3] = lightColor[3];
    
    m_vPosition[0] = pos[0];
    m_vPosition[1] = pos[1];
    m_vPosition[2] = pos[2];
     
    // Check if this shader has already been created, otherwise create it.
    ShaderManager *shaderManager = Manager::shaderManager();
    Shader *shader = shaderManager->getShaderForEffectType(TEXTURE_POINT_LIGHT_EFFECT);
    if (shader) {
        m_pEffect = (GLTexturePointLightDiffuseEffect*) shader->m_pEffect;
    }
    else {
        m_pEffect = new GLTexturePointLightDiffuseEffect;
        m_pEffect->init();
        shaderManager->setShaderForEffectType(TEXTURE_POINT_LIGHT_EFFECT, m_pEffect);
    }

    char texFileName[100];
    
    GLfloat *vertices;
    GLushort *indices;
    ObjLoader loader;
    int numVertices;
    int numIndices = loader.loadModel(model, &vertices, &indices, &numVertices, texFileName, &m_fBoundingRadius, scale);
    
    GeometryManager *geoManager = Manager::geometryManager();
    Geometry* geometry = geoManager->getGeometryForEntityType(m_type);
    if (geometry) {
        m_pBatch = geometry->m_pBatch1;
    }
    else {
        m_pBatch = new GLBatch;
        geoManager->setGeometryForEntityType((EntityType)m_type,
                                          m_pBatch,
                                          0,
                                          0);
        
        m_pBatch->beginInterleaved(GL_TRIANGLES, numVertices, 0);
        m_pBatch->copyIndices(indices, numIndices);
        m_pBatch->copyInterleavedVertexData(vertices);
        m_pBatch->endInterleaved();
    }

    delete[] vertices;
    delete[] indices;
    
    // Load texture.
    glGenTextures(1, &m_uiTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
    
    if (GLUtils::loadPNGTexture(texFileName, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("%s texture loaded\n", texFileName);
    }
}

void Station::update(float dt)
{
    Camera *camera = Manager::camera();
    
    m_modelViewMatrix = camera->viewMatrix();
    m_pointModelViewMatrix = camera->viewMatrix();
    
    Matrix4 matrix = camera->viewMatrix();    
    Vector4::transformVector4(m_vLightEyePos, m_vLightPos, matrix);
    
    Vector3d campos;
    camera->getPosition(campos);
    
    m_vRelPosition[0] = m_vPosition[0] + campos[0];
    m_vRelPosition[1] = m_vPosition[1] + campos[1];
    m_vRelPosition[2] = m_vPosition[2] + campos[2];
    
    m_modelViewMatrix.translate(m_vRelPosition);
    m_pointModelViewMatrix.translate(m_vRelPosition);
    
    m_modelViewMatrix.rotate(orientation[0], 1.0f, 0.0f, 0.0f);
    m_modelViewMatrix.rotate(orientation[1], 0.0f, 1.0f, 0.0f);
    m_modelViewMatrix.rotate(orientation[2], 0.0f, 0.0f, 1.0f);
}

void Station::render()
{  
    m_pEffect->setMin(0.3);
    m_pEffect->setLightColor(m_vLightColor);
    Vector4f lightPos;
    
    lightPos[0] = m_vLightEyePos[0];
    lightPos[1] = m_vLightEyePos[1];
    lightPos[2] = m_vLightEyePos[2];
    
    m_pEffect->setLightPos(lightPos);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
    
    m_pEffect->setMatrices(Manager::camera()->projectionMatrixPerspective(), m_modelViewMatrix);
    m_pEffect->prepareToRender();
    
    m_pBatch->renderInterleaved();
}
