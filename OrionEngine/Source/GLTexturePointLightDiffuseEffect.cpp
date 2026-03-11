//
//  GLTexturePointLightDiffuseEffect.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-14.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GLTexturePointLightDiffuseEffect.h"
#include "ShaderUtils.h"
#include "Constants.h"
#include <cmath>

GLTexturePointLightDiffuseEffect::GLTexturePointLightDiffuseEffect()
{
    
}

GLTexturePointLightDiffuseEffect::~GLTexturePointLightDiffuseEffect()
{
    glDeleteProgram(m_uiProgramObject);    
}

bool GLTexturePointLightDiffuseEffect::init()
{
    std::string vert = "#define VERTEX_SHADER\n";
    std::string frag = "#define FRAGMENT_SHADER\n";
    
#ifdef OPENGL_ES
    frag += "#define OPENGL_ES\n";
#endif
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/TexturePointLightDiffuseEffect.glsl",
                                                                 &vert,
                                                                 &frag,
                                                                 3,
                                                                 VertexAttribVertex, "vVertex",
                                                                 VertexAttribNormal, "vNormal",
                                                                 VertexAttribTexCoord0, "vTexCoord0");
    if (m_uiProgramObject == 0)
        return false;
    
    // Bind uniforms
    m_iLocTransformP = glGetUniformLocation(m_uiProgramObject, "pMatrix");
    m_iLocTransformM = glGetUniformLocation(m_uiProgramObject, "mvMatrix");
    m_iLocColor = glGetUniformLocation(m_uiProgramObject, "vColor");
    m_iLocLight = glGetUniformLocation(m_uiProgramObject, "vLightPos");
    m_iLocTextureUnit = glGetUniformLocation(m_uiProgramObject, "textureUnit");
    
    m_iLocNear = glGetUniformLocation(m_uiProgramObject, "near");
    m_iLocFar = glGetUniformLocation(m_uiProgramObject, "far");
    
    m_fMin = 0.0f;
    m_iLocMin = glGetUniformLocation(m_uiProgramObject, "min");
    
    return true;    
}

void GLTexturePointLightDiffuseEffect::prepareToRender()
{
    glUseProgram(m_uiProgramObject);
    
    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    glUniform3fv(m_iLocLight, 1, m_vLightPos);
    glUniform4fv(m_iLocColor, 1, m_vColor);
    glUniform1i(m_iLocTextureUnit, 0);
    glUniform1f(m_iLocNear, kNear);
    glUniform1f(m_iLocFar, kFar);
    glUniform1f(m_iLocMin, m_fMin);
}

void GLTexturePointLightDiffuseEffect::setLightPos(Vector3f lightPos)
{
    m_vLightPos[0] = lightPos[0];
    m_vLightPos[1] = lightPos[1];
    m_vLightPos[2] = lightPos[2];    
}

void GLTexturePointLightDiffuseEffect::setLightColor(Vector4f lightColor)
{
    m_vColor[0] = lightColor[0];
    m_vColor[1] = lightColor[1];
    m_vColor[2] = lightColor[2];
    m_vColor[3] = lightColor[3];
}
