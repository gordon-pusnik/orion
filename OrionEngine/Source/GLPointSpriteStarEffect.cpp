//
//  GLPointSpriteStarEffect.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GLPointSpriteStarEffect.h"
#include "ShaderUtils.h"
#include "Utils.h"
#include <cstdlib>
#include "Constants.h"
#include "AssetManager.h"
#include <string>


GLPointSpriteStarEffect::GLPointSpriteStarEffect()
{
    
}

GLPointSpriteStarEffect::~GLPointSpriteStarEffect()
{
    glDeleteProgram(m_uiProgramObject);
}

bool GLPointSpriteStarEffect::init()
{    
#ifdef OPENGL_ES
    std::string vert = "#define VERTEX_SHADER\n";
    std::string frag = "#define FRAGMENT_SHADER\n";
    vert += "#define OPENGL_ES\n";
    frag += "#define OPENGL_ES\n";
#else
    std::string vert = "#version 120\n#define VERTEX_SHADER\n";
    std::string frag = "#version 120\n#define FRAGMENT_SHADER\n";
#endif
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/PointSpriteStarEffect.glsl",
                                                                 &vert,
                                                                 &frag,
                                                                 2,
                                                                 VertexAttribVertex, "vVertex",
                                                                 VertexAttribColor, "vColor");
    if (m_uiProgramObject == 0)
        return false;

    // Bind uniforms
    m_iLocTransformP = glGetUniformLocation(m_uiProgramObject, "pMatrix");
    m_iLocTransformM = glGetUniformLocation(m_uiProgramObject, "mvMatrix");
    m_iLocTextureUnit = glGetUniformLocation(m_uiProgramObject, "image");

    m_iLocNear = glGetUniformLocation(m_uiProgramObject, "near");
    m_iLocFar = glGetUniformLocation(m_uiProgramObject, "far");
    
    return true;
}

void GLPointSpriteStarEffect::prepareToRender()
{
    glUseProgram(m_uiProgramObject);

    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    glUniform1i(m_iLocTextureUnit, 0);
    glUniform1f(m_iLocNear, kNear);
    glUniform1f(m_iLocFar, kFar);
}
