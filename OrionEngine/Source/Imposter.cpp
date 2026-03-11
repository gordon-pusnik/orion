//
//  Imposter.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-19.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "Imposter.h"
#include "Manager.h"
#include "GLUtils.h"
#include "Utils.h"

Imposter::Imposter()
{
    m_bFaceCamera = true;
    m_fRotX = 0.0;
    m_fRotY = 0.0;
    m_fScale = 1.0;
}

Imposter::~Imposter()
{
    delete m_pEffect;
    delete m_pTriangleBatch;
    glDeleteTextures(1, &m_uiTextureID);
}

void Imposter::rotate(float rotx, float roty)
{
    m_fRotX = rotx;
    m_fRotY = roty;
}

void Imposter::scale(float scale)
{
    m_fScale = scale;
}

void Imposter::init(Vector3f position, float size, const char *texture, BlendMode mode, bool faceCamera, bool cullFace)
{
    m_blendMode = mode;
    m_bFaceCamera = faceCamera;
    m_bCullFace = cullFace;
    
    m_vPosition[0] = position[0];
    m_vPosition[1] = position[1];
    m_vPosition[2] = position[2];
    
    m_fZPos = m_vPosition[2];
    
    glGenTextures(1, &m_uiTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
    
    if (GLUtils::loadPNGTexture(texture, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("Texture %s loaded\n", texture);
    }
    
    m_pEffect = new GLTextureEffect;
    m_pEffect->init();
    
    GLfloat *pVertices;
    GLushort *pIndices;
    int numVertices;
    int numIndices = GLUtils::genSquare(size, &pVertices, &pIndices, &numVertices);
    
    m_pTriangleBatch = new GLBatch;
    m_pTriangleBatch->beginInterleaved(GL_TRIANGLES, numVertices, 0);
    m_pTriangleBatch->copyIndices(pIndices, numIndices);
    m_pTriangleBatch->copyInterleavedVertexData(pVertices);
    m_pTriangleBatch->endInterleaved();
    
    delete[] pVertices;
    delete[] pIndices;
}

void Imposter::update(double dt)
{
    Camera *camera = Manager::camera();
    
    Matrix4 rotationMatrix = camera->rotationMatrix();
    
    m_vPosition[2] = m_fZPos * m_fScale;
    
    rotationMatrix.translate(m_vPosition);
    
    rotationMatrix.rotate(m_fRotX, 1.0f, 0.0f, 0.0f);
    rotationMatrix.rotate(m_fRotY, 0.0f, 1.0f, 0.0f);
    
    if (m_bFaceCamera) {
        // Face it to the the camera.
        Vector3f viewCoords;
        camera->getViewCoords(viewCoords);
        
        Vector3f zaxis;
        zaxis[0] = viewCoords[0];
        zaxis[1] = viewCoords[1];
        zaxis[2] = viewCoords[2];
        Vector3::normalize(zaxis);
        
        Vector3f xaxis;
        Vector3f u;
        u[0] = 0.0f;
        u[1] = 1.0f;
        u[2] = 0.0f;
        Vector3::crossProduct(xaxis, u, zaxis);
        Vector3::normalize(xaxis);
        
        Vector3f yaxis;
        Vector3::crossProduct(yaxis, zaxis, xaxis);
        
        Matrix4 rot = Matrix4::makeRotationMatrix(xaxis, yaxis, zaxis);
        rot = rot.inverseOf();
        
        rotationMatrix.multiply(rot);
    }
    
    m_pEffect->setMatrices(camera->projectionMatrixPerspective(), rotationMatrix);    
}

void Imposter::render()
{
    Manager::renderer()->enableCullFace(m_bCullFace);
    Manager::renderer()->enableDepthTest(false);
    Manager::renderer()->setBlendMode(m_blendMode);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
    
    m_pEffect->prepareToRender();
    m_pTriangleBatch->renderInterleaved();

    Manager::renderer()->setBlendMode(BLEND_ALPHA);
    Manager::renderer()->enableDepthTest(true);
    Manager::renderer()->enableCullFace(true);
}