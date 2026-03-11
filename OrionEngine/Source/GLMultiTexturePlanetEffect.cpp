//
//  GLMultiTexturePlanetEffect.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-14.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GLMultiTexturePlanetEffect.h"
#include "ShaderUtils.h"
#include "Constants.h"
#include <cmath>

GLMultiTexturePlanetEffect::GLMultiTexturePlanetEffect()
{
    
}

GLMultiTexturePlanetEffect::~GLMultiTexturePlanetEffect()
{
    glDeleteProgram(m_uiProgramObject);    
}

bool GLMultiTexturePlanetEffect::init()
{
    std::string vert = "#define VERTEX_SHADER\n";
    std::string frag = "#define FRAGMENT_SHADER\n";
    
#ifdef OPENGL_ES
    frag += "#define OPENGL_ES\n";
#endif
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/MultiTexturePlanetEffect.glsl",
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
    m_iLocTextureUnit1 = glGetUniformLocation(m_uiProgramObject, "textureUnit1");
    m_iLocTextureUnit2 = glGetUniformLocation(m_uiProgramObject, "textureUnit2");
    m_iLocTextureUnit3 = glGetUniformLocation(m_uiProgramObject, "textureUnit3");
    
    m_iLocNear = glGetUniformLocation(m_uiProgramObject, "near");
    m_iLocFar = glGetUniformLocation(m_uiProgramObject, "far");
    
    return true;    
}

void GLMultiTexturePlanetEffect::prepareToRender()
{
    glUseProgram(m_uiProgramObject);
    
    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    glUniform3fv(m_iLocLight, 1, m_vLightPos);
    glUniform4fv(m_iLocColor, 1, m_vColor);
    glUniform1i(m_iLocTextureUnit1, 0);
    glUniform1i(m_iLocTextureUnit2, 1);
    glUniform1i(m_iLocTextureUnit3, 2);
    glUniform1f(m_iLocNear, kNear);
    glUniform1f(m_iLocFar, kFar);
}

void GLMultiTexturePlanetEffect::setLightPos(Vector3f lightPos)
{
    m_vLightPos[0] = lightPos[0];
    m_vLightPos[1] = lightPos[1];
    m_vLightPos[2] = lightPos[2];    
}

void GLMultiTexturePlanetEffect::setLightColor(Vector4f lightColor)
{
    m_vColor[0] = lightColor[0];
    m_vColor[1] = lightColor[1];
    m_vColor[2] = lightColor[2];
    m_vColor[3] = lightColor[3];
}
