//
//  GLCubemapEffect.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-18.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GLCubemapEffect.h"
#include "ShaderUtils.h"
#include "Utils.h"
#include <cstdlib>

GLCubemapEffect::GLCubemapEffect()
{
    
}

GLCubemapEffect::~GLCubemapEffect()
{
    glDeleteProgram(m_uiProgramObject);
}

bool GLCubemapEffect::init()
{
    std::string vert = "#define VERTEX_SHADER\n";
    std::string frag = "#define FRAGMENT_SHADER\n";
    
#ifdef OPENGL_ES
    frag += "#define OPENGL_ES\n";
#endif
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/CubemapEffect.glsl",
                                                                 &vert,
                                                                 &frag,
                                                                 1,
                                                                 VertexAttribVertex, "vVertex");
    if (m_uiProgramObject == 0)
        return false;
    
    // Bind uniforms
    m_iLocTransformP = glGetUniformLocation(m_uiProgramObject, "pMatrix");
    m_iLocTransformM = glGetUniformLocation(m_uiProgramObject, "mvMatrix");
    m_iLocCubemap = glGetUniformLocation(m_uiProgramObject, "cubemap");
    
    return true;
}

void GLCubemapEffect::prepareToRender()
{
    glUseProgram(m_uiProgramObject);
    
    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    glUniform1i(m_iLocCubemap, 4);
}
