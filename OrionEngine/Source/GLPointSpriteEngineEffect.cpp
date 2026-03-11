//
//  GLPointSpriteEngineEffect.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GLPointSpriteEngineEffect.h"
#include "ShaderUtils.h"
#include "Utils.h"
#include <cstdlib>
#include "Constants.h"
#include "AssetManager.h"
#include <string>


GLPointSpriteEngineEffect::GLPointSpriteEngineEffect()
{
    m_fPower = 0.0;
}

GLPointSpriteEngineEffect::~GLPointSpriteEngineEffect()
{
    glDeleteProgram(m_uiProgramObject);
}

bool GLPointSpriteEngineEffect::init()
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
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/PointSpriteEngineEffect.glsl",
                                                                 &vert,
                                                                 &frag,
                                                                 1,
                                                                 VertexAttribVertex, "vVertex");
    if (m_uiProgramObject == 0)
        return false;

    // Bind uniforms
    m_iLocTransformP = glGetUniformLocation(m_uiProgramObject, "pMatrix");
    m_iLocTransformM = glGetUniformLocation(m_uiProgramObject, "mvMatrix");
    m_iLocTextureUnit = glGetUniformLocation(m_uiProgramObject, "textureUnit");

    m_iLocNear = glGetUniformLocation(m_uiProgramObject, "near");
    m_iLocFar = glGetUniformLocation(m_uiProgramObject, "far");
    
    m_iLocColor = glGetUniformLocation(m_uiProgramObject, "vColor");
    m_iLocPower = glGetUniformLocation(m_uiProgramObject, "power");
    
    return true;
}

void GLPointSpriteEngineEffect::prepareToRender()
{
    glUseProgram(m_uiProgramObject);

    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    glUniform1i(m_iLocTextureUnit, 0);
    glUniform1f(m_iLocNear, kNear);
    glUniform1f(m_iLocFar, kFar);
    glUniform4fv(m_iLocColor, 1, m_fColor);
    glUniform1f(m_iLocPower, m_fPower);
}
