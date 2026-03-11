//
//  Starfield.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-12-27.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Starfield.h"
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

Starfield::Starfield()
{
    
}

Starfield::~Starfield()
{
    delete m_pEffect;
    delete m_pPointsBatch;
    glDeleteTextures(1, &m_uiTextureID);
}

void Starfield::init()
{
    int numStars = 100000;
    GLfloat *vertexes = new GLfloat[numStars * 7];
    int j = 0;
    for (int i = 0; i < numStars; i++) {
        float theta = float(Random::randomInRange(0.0, 2.0 * kPi));
        float u = float(Random::randomInRange(-1.0, 1.0));
        
        float x = Utils::getScaled(10.e12 * sqrt(1.0f - u * u) * cos(theta));
        float y = Utils::getScaled(10.e12 * u);
        float z = Utils::getScaled(10.e12 * sqrt(1.0f - u * u) * sin(theta));
        
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
    
    if (GLUtils::loadPNGTexture("Images/halo.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("Starfield texture loaded\n");
    }

    m_vPosition[0] = 0.0;
    m_vPosition[1] = 0.0;
    m_vPosition[2] = 0.0;
/*
    int numIndices = numStars;
    GLushort *pIndices = new GLushort[numIndices];
    
    for (int i = 0; i < numIndices; i++) {
        pIndices[i] = i;
    }
*/    
    m_pEffect = new GLPointSpriteStarEffect;
    m_pEffect->init();
    m_pPointsBatch = new GLBatch;
    m_pPointsBatch->beginInterleaved(GL_POINTS, numStars, 0);
//    m_pPointsBatch->copyIndices(pIndices, numIndices);
    m_pPointsBatch->copyInterleavedVertexColorData(vertexes);
    m_pPointsBatch->endInterleavedColor();
    
    delete[] vertexes;
//    delete[] pIndices;
}

void Starfield::update(double dt)
{
    Camera *camera = Manager::camera();
    
    Matrix4 rotationMatrix = camera->rotationMatrix();
    rotationMatrix.translate(m_vPosition);
    
    m_pEffect->setMatrices(camera->projectionMatrixPerspective(), rotationMatrix);
}

void Starfield::render()
{
    Renderer *renderer = Manager::renderer();
    renderer->setBlendMode(BLEND_ADDITIVE);
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
