//
//  GLTextureEffect.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-14.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GLTextureEffect.h"
#include "ShaderUtils.h"
#include "Constants.h"
#include <cmath>
#include "Utils.h"

GLTextureEffect::GLTextureEffect()
{
    
}

GLTextureEffect::~GLTextureEffect()
{
    glDeleteProgram(m_uiProgramObject);    
}

bool GLTextureEffect::init()
{
    std::string vert = "#define VERTEX_SHADER\n";
    std::string frag = "#define FRAGMENT_SHADER\n";
    
#ifdef OPENGL_ES
    frag += "#define OPENGL_ES\n";
#endif
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/TextureEffect.glsl",
                                                                 &vert,
                                                                 &frag,
                                                                 2,
                                                                 VertexAttribVertex, "vVertex",
                                                                 VertexAttribTexCoord0, "vTexCoord0");
    if (m_uiProgramObject == 0)
        return false;
    
    // Bind uniforms
    m_iLocTransformP = glGetUniformLocation(m_uiProgramObject, "pMatrix");
    m_iLocTransformM = glGetUniformLocation(m_uiProgramObject, "mvMatrix");
    m_iLocTextureUnit = glGetUniformLocation(m_uiProgramObject, "textureUnit");

    m_iLocNear = glGetUniformLocation(m_uiProgramObject, "near");
    m_iLocFar = glGetUniformLocation(m_uiProgramObject, "far");

    return true;    
}

void GLTextureEffect::prepareToRender()
{
    glUseProgram(m_uiProgramObject);
    
    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    glUniform1i(m_iLocTextureUnit, 0);
    glUniform1f(m_iLocNear, kNear);
    glUniform1f(m_iLocFar, kFar);
}
