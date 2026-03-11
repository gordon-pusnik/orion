//
//  GLCoronaEffect.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GLCoronaEffect.h"
#include "ShaderUtils.h"
#include "Utils.h"
#include <cstdlib>
#include "Constants.h"
#include "AssetManager.h"
#include <string>


GLCoronaEffect::GLCoronaEffect()
{
    
}

GLCoronaEffect::~GLCoronaEffect()
{
    glDeleteProgram(m_uiProgramObject);
}

bool GLCoronaEffect::init()
{    
    std::string vert = "#define VERTEX_SHADER\n";
    std::string frag = "#define FRAGMENT_SHADER\n";

#ifdef OPENGL_ES
    frag += "#define OPENGL_ES\n";
#endif
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/CoronaEffect.glsl",
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
    m_iLocTime = glGetUniformLocation(m_uiProgramObject, "fTime");

    m_iLocNear = glGetUniformLocation(m_uiProgramObject, "near");
    m_iLocFar = glGetUniformLocation(m_uiProgramObject, "far");
    
    return true;
}

void GLCoronaEffect::prepareToRender()
{
    glUseProgram(m_uiProgramObject);

    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    glUniform1f(m_iLocTime, m_fTime);
    glUniform1f(m_iLocNear, kNear);
    glUniform1f(m_iLocFar, kFar);
}
