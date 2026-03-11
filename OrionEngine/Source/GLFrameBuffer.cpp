//
//  GLFrameBuffer.cpp
//  Particlizer
//
//  Created by Gordon Pusnik on 2013-02-10.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "GLFrameBuffer.h"
#include "Utils.h"
#include "Manager.h"
#include "GLUtils.h"

GLFrameBuffer::GLFrameBuffer()
{
}

GLFrameBuffer::~GLFrameBuffer()
{
    glDeleteFramebuffers(1, &m_uiFboID);
    glDeleteRenderbuffers(1, &m_uiDepthBufferID);
    glDeleteTextures(1, &m_uiTextureID);
}

void GLFrameBuffer::init(float viewWidth, float viewHeight)
{
    m_fViewWidth = viewWidth;
    m_fViewHeight = viewHeight;
    
    m_vPosition[0] = 0.0;
    m_vPosition[1] = 0.0;
    m_vPosition[2] = 0.0;
    
    GLfloat vertices[] =
    {
        viewWidth, 0, 0,
        viewWidth, viewHeight, 0,
        0, viewHeight, 0,
        0, 0, 0
    };

    GLfloat texCoords[] =
    {
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
    };
    
    // Make interleaved
    int numVertices = 4;
    GLfloat *vertexes = new GLfloat[5 * numVertices];
    
    int i = 0;
    int j = 0;
    int k = 0;
    
    for (int vert = 0; vert < numVertices; vert++) {
        vertexes[i++] = vertices[j++];
        vertexes[i++] = vertices[j++];
        vertexes[i++] = vertices[j++];
        vertexes[i++] = texCoords[k++];
        vertexes[i++] = texCoords[k++];
    }
    
    GLushort squareIndices[] =
    {
        0, 1, 2,
        0, 2, 3
    };
    
    GeometryManager *geoManager = Manager::geometryManager();
    Geometry* geometry = geoManager->getGeometryForEntityType((EntityType)1000);
    if (geometry) {
        m_pBatch = geometry->m_pBatch1;
    }
    else {
        m_pBatch = new GLBatch;
        geoManager->setGeometryForEntityType((EntityType)1000,
                                          m_pBatch,
                                          0,
                                          0);
        
        m_pBatch->beginInterleaved(GL_TRIANGLES, numVertices, 0);
        m_pBatch->copyInterleavedVertexTexCoordData((GLfloat*)vertexes);
        m_pBatch->copyIndices(squareIndices, 6);
        m_pBatch->endInterleavedVertexTexCoord();
        
        delete[] vertexes;
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

    // Create framebuffer
    glGenFramebuffers(1, &m_uiFboID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiFboID);

    glGenRenderbuffers(1, &m_uiDepthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthBufferID);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, viewWidth, viewHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiDepthBufferID);

    // Create and attach texture to frame buffer.
    glGenTextures(1, &m_uiTextureID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
    
    // Set up texture maps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewWidth, viewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTextureID, 0);

    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGI("Init framebuffer failed\n");
        glDeleteFramebuffers(1, &m_uiFboID);
        glDeleteRenderbuffers(1, &m_uiDepthBufferID);
        glDeleteTextures(1, &m_uiTextureID);
    }
    else {
        LOGI("Init framebuffer suceeded\n");
    }

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiFboID);
    glViewport(0, 0, m_fViewWidth, m_fViewHeight);
}

void GLFrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::render()
{
    Matrix4 modelViewMatrix = Matrix4::makeTranslationMatrix(m_vPosition);
    m_pEffect->setMatrices(Manager::camera()->projectionMatrixOrtho(), modelViewMatrix);
    
    Manager::renderer()->enableDepthTest(false);
    Manager::renderer()->clearScreen();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

    m_pEffect->prepareToRender();
    
    m_pBatch->renderInterleavedVertexTexCoord();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    Manager::renderer()->enableDepthTest(true);
}
