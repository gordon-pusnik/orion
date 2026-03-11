//
//  Renderer.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-31.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Renderer.h"
#include "GLUtils.h"
#include "Utils.h"

#ifdef __APPLE__
#ifndef OPENGL_ES
#include <OpenGL/gl.h>
#endif
#endif

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::init(bool hirez)
{
    GLUtils::printGLString("Version", GL_VERSION);
    GLUtils::printGLString("Vendor", GL_VENDOR);
    GLUtils::printGLString("Renderer", GL_RENDERER);
    GLUtils::printGLString("Extensions", GL_EXTENSIONS);

    glGetIntegerv(GL_DEPTH_BITS, &m_iGlDepth);
    LOGI("depth bits: %d, %d\n", m_iGlDepth, (int)hirez);
    
    m_bHiRez = hirez;
    
    GLint numAttribs = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttribs);
    
    LOGI("max attributes: %d\n", numAttribs);
}

void Renderer::setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    glClearColor(r, g, b, a);
}

void Renderer::clearScreen()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Renderer::setViewport(int x, int y, int w, int h)
{
    glViewport(0, 0, w, h);
    m_iViewPortWidth = w;
    m_iViewPortHeight = h;
}

void Renderer::enableDepthTest(bool b)
{
    if (b) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::enableCullFace(bool b)
{
    if (b) {
        glEnable(GL_CULL_FACE);
    }
    else {
        glDisable(GL_CULL_FACE);
    }
}

bool Renderer::setBlendMode(BlendMode m)
{
	switch (m) {
        case BLEND_SOLID:
            glDisable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ZERO);
            break;
        case BLEND_ADDITIVE:
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            break;
        case BLEND_ALPHA:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case BLEND_ALPHA_ONE:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        case BLEND_ALPHA_PREMULT:
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
            break;
        default:
            return false;
	}
	return true;
}