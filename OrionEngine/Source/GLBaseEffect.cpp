//
//  GLBaseEffect.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GLBaseEffect.h"
#include "ShaderUtils.h"
#include "Utils.h"
#include <cstdlib>
#include "Constants.h"
#include "AssetManager.h"
#include <string>

GLBaseEffect::GLBaseEffect()
{
    
}

GLBaseEffect::~GLBaseEffect()
{
    glDeleteProgram(m_uiProgramObject);
}

bool GLBaseEffect::init()
{
    std::string vert = "#define VERTEX_SHADER\n";
    std::string frag = "#define FRAGMENT_SHADER\n";

#ifdef OPENGL_ES
    frag += "#define OPENGL_ES\n";
#endif
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/BaseEffect.glsl",
                                                                 &vert,
                                                                 &frag,
                                                                 1,
                                                                 VertexAttribVertex, "vVertex");
    if (m_uiProgramObject == 0)
        return false;

    // Bind uniforms
    m_iLocTransformP = glGetUniformLocation(m_uiProgramObject, "pMatrix");
    m_iLocTransformM = glGetUniformLocation(m_uiProgramObject, "mvMatrix");
    m_iLocColor = glGetUniformLocation(m_uiProgramObject, "vColor");

    m_iLocNear = glGetUniformLocation(m_uiProgramObject, "near");
    m_iLocFar = glGetUniformLocation(m_uiProgramObject, "far");
    
    m_fColor[0] = 1.0;
    m_fColor[1] = 1.0;
    m_fColor[2] = 1.0;
    m_fColor[3] = 1.0;
    
    return true;
}

void GLBaseEffect::prepareToRender()
{
    glUseProgram(m_uiProgramObject);

    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    glUniform1f(m_iLocNear, kNear);
    glUniform1f(m_iLocFar, kFar);
    glUniform4fv(m_iLocColor, 1, m_fColor);
}

void GLBaseEffect::setColor(Vector4f color)
{
    m_fColor[0] = color[0];
    m_fColor[1] = color[1];
    m_fColor[2] = color[2];
    m_fColor[3] = color[3];
}

void GLBaseEffect::setMatrices(Matrix4 projectionMatrix, Matrix4 modelViewMatrix)
{
    m_transform.projectionMatrix = projectionMatrix;
    m_transform.modelViewMatrix = modelViewMatrix;
}
