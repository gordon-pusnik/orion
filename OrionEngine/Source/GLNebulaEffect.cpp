//
//  GLNebulaEffect.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-19.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "GLNebulaEffect.h"
#include "ShaderUtils.h"
#include "Constants.h"
#include <cmath>
#include "Utils.h"

GLNebulaEffect::GLNebulaEffect()
{
    
}

GLNebulaEffect::~GLNebulaEffect()
{
    glDeleteProgram(m_uiProgramObject);    
}

bool GLNebulaEffect::init()
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
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/NebulaEffect.glsl",
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
    
    m_iLocLightPos = glGetUniformLocation(m_uiProgramObject, "LightPosition");
    m_iLocNoiseScale = glGetUniformLocation(m_uiProgramObject, "NoiseScale");
    m_iLocScale = glGetUniformLocation(m_uiProgramObject, "scale");
    m_iLocRelativeSize = glGetUniformLocation(m_uiProgramObject, "relativeSize");
    m_iLocColor1 = glGetUniformLocation(m_uiProgramObject, "Color1");
    m_iLocColor2 = glGetUniformLocation(m_uiProgramObject, "Color2");
    m_iLocNoiseTexture = glGetUniformLocation(m_uiProgramObject, "Noise");

    m_iLocNear = glGetUniformLocation(m_uiProgramObject, "near");
    m_iLocFar = glGetUniformLocation(m_uiProgramObject, "far");
    
    m_fColor[0] = 0.010;
    m_fColor[1] = 0.007;
    m_fColor[2] = 0.020;
    m_fColor[3] = 1.0;

    return true;    
}

void GLNebulaEffect::prepareToRender()
{
    glUseProgram(m_uiProgramObject);
    
    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    
    glUniform3f(m_iLocLightPos, 0.0, 0.0, 4.0);
    glUniform1f(m_iLocNoiseScale, 1.2);
    glUniform1f(m_iLocScale, m_fScale);
    glUniform1f(m_iLocRelativeSize, m_fRelativeSize);
//    glUniform4f(m_iLocColor1, 0.010, 0.007, 0.010, 1.0);
//    glUniform4f(m_iLocColor1, 0.010, 0.007, 0.020, 1.0);
    glUniform4fv(m_iLocColor1, 1, m_fColor);
    glUniform4f(m_iLocColor2, 0.6, 0.1, 0.0, 1.0);
    glUniform1i(m_iLocNoiseTexture, 0);
    glUniform1f(m_iLocNear, kNear);
    glUniform1f(m_iLocFar, kFar);
}
