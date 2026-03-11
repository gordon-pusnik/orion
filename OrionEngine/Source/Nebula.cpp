//
//  Nebula.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-21.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "Nebula.h"
#include "Manager.h"
#include "Utils.h"
#include "Constants.h"
#include "GLUtils.h"
#include "Random.h"

#ifdef __APPLE__
#ifndef OPENGL_ES
#include <OpenGL/gl.h>
#endif
#endif

Nebula::Nebula()
{
    m_fPanX = 0.0;
    m_fPanY = 0.0;
    m_fRotX = 0.0;
    m_fRotY = 0.0;
    m_fRotZ = 0.0;
    m_fScale = 1.0;
    m_fRelativeSize = 500.0;
}

Nebula::~Nebula()
{
    delete m_pEffect;
    delete m_pPointsBatch;
    glDeleteTextures(1, &m_uiTextureID);
}

void Nebula::rotate(float rotx, float roty, float rotz)
{
    m_fRotX = rotx;
    m_fRotY = roty;
    m_fRotZ = rotz;
}

void Nebula::scale(float scale)
{
    m_fScale = scale;
}

void Nebula::pan(float x, float y)
{
    m_fPanX = x;
    m_fPanY = y;
}

void Nebula::init(Vector3f pos, int num, float xsize, float ysize, float particleSize, Vector4f color, BlendMode mode, const char* tex)
{
    m_blendMode = mode;
    m_fRelativeSize = particleSize;
    
    GLfloat *vertexes = new GLfloat[num * 7];
    
    int j = 0;
    for (int i = 0; i < num; i++) {
        float phi = float(Random::randomInRange(0.0, 2.0 * kPi));
        float rho = float(Random::randomInRange(0.0, 1.0));
        
        float x = sqrt(rho) * cos(phi);
        float y = sqrt(rho) * sin(phi);
        
        x = x * xsize / 2.0;
        y = y * ysize / 2.0;
        
        //x += 1000;
        
        float z = 0.0;
        
        //        LOGI("x: %f, y: %f, z: %f\n", x, y, z);
        
        float r = Random::randomInRange(0.7, 1.0);
        float g = Random::randomInRange(0.7, 1.0);
        float b = Random::randomInRange(0.7, 1.0);
        float a = Random::randomInRange(0.2, 1.0);
        
        //        LOGI("r: %f, g: %f, b: %f, a: %f\n", r, g, b, a);
        
        vertexes[j++] = x;
        vertexes[j++] = y;
        vertexes[j++] = z;
        vertexes[j++] = r;
        vertexes[j++] = g;
        vertexes[j++] = b;
        vertexes[j++] = a;
    }
    
    glGenTextures(1, &m_uiTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

#ifndef OPENGL_ES
    glEnable(GL_POINT_SPRITE);
#endif
    
    if (GLUtils::loadPNGTexture(tex, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("Nebula texture loaded\n");
    }
    
    m_vPosition[0] = pos[0];
    m_vPosition[1] = pos[1];
    m_vPosition[2] = pos[2];
    
    m_fZPos = m_vPosition[2];
    
    /*
     int numIndices = numStars;
     GLushort *pIndices = new GLushort[numIndices];
     
     for (int i = 0; i < numIndices; i++) {
     pIndices[i] = i;
     }
     */
    m_pEffect = new GLNebulaEffect;
    m_pEffect->init();
    m_pEffect->setColor(color);
    m_pPointsBatch = new GLBatch;
    m_pPointsBatch->beginInterleaved(GL_POINTS, num, 0);
    //    m_pPointsBatch->copyIndices(pIndices, numIndices);
    m_pPointsBatch->copyInterleavedVertexColorData(vertexes);
    m_pPointsBatch->endInterleavedColor();
    
    delete[] vertexes;
    //    delete[] pIndices;
}

void Nebula::update(double dt)
{
    Camera *camera = Manager::camera();
    
    Matrix4 rotationMatrix = camera->rotationMatrix();
    
    m_vPosition[0] = m_fPanX;
    m_vPosition[1] = m_fPanY;
    m_vPosition[2] = m_fZPos * m_fScale;
    
    rotationMatrix.translate(m_vPosition);
    
    rotationMatrix.rotate(m_fRotX, 1.0f, 0.0f, 0.0f);
    rotationMatrix.rotate(m_fRotY, 0.0f, 1.0f, 0.0f);
    rotationMatrix.rotate(m_fRotZ, 0.0f, 0.0f, 1.0f);
    
    m_pEffect->setScale(m_fScale);
    m_pEffect->setRelativeSize(m_fRelativeSize);
    
    m_pEffect->setMatrices(camera->projectionMatrixPerspective(), rotationMatrix);
}

void Nebula::render()
{
    Renderer *renderer = Manager::renderer();
    renderer->setBlendMode(m_blendMode);
    renderer->enableDepthTest(false);
#ifndef OPENGL_ES
    glEnable(GL_PROGRAM_POINT_SIZE);
#endif
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
    m_pEffect->prepareToRender();
    m_pPointsBatch->renderInterleavedColor();
    
    // Restore render state to normal state
    renderer->setBlendMode(BLEND_ALPHA);
    renderer->enableDepthTest(true);
#ifndef OPENGL_ES
    glDisable(GL_PROGRAM_POINT_SIZE);
#endif
}
