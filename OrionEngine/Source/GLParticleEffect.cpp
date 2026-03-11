//
//  GLParticleEffect.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "GLParticleEffect.h"
#include "ShaderUtils.h"
#include "Utils.h"
#include <cstdlib>
#include "Constants.h"
#include "AssetManager.h"
#include <string>


GLParticleEffect::GLParticleEffect()
{
    m_vEmitterPos[0] = 0.0;
    m_vEmitterPos[1] = 0.0;
    m_vEmitterPos[2] = 0.0;
    
    m_vVelocity[0] = 0.0;
    m_vVelocity[1] = 0.0;
    m_vVelocity[2] = 0.0;
    
    m_fType = 0.0;
    
    m_fTime = 0.0;
    
    m_fColor1[0] = 1.0;
    m_fColor1[1] = 1.0;
    m_fColor1[2] = 1.0;
    m_fColor1[3] = 1.0;
    
    m_fColor2[0] = 1.0;
    m_fColor2[1] = 1.0;
    m_fColor2[2] = 1.0;
    m_fColor2[3] = 1.0;    
}

GLParticleEffect::~GLParticleEffect()
{
    glDeleteProgram(m_uiProgramObject);
}

bool GLParticleEffect::init()
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
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/ParticleEffect.glsl",
                                                                 &vert,
                                                                 &frag,
                                                                 2,
                                                                 VertexAttribVertex, "initialVelocity",
                                                                 VertexAttribTime, "startTime");
    if (m_uiProgramObject == 0)
        return false;

    // Bind uniforms
    m_iLocTransformP = glGetUniformLocation(m_uiProgramObject, "pMatrix");
    m_iLocTransformM = glGetUniformLocation(m_uiProgramObject, "mvMatrix");
    m_iLocTextureUnit = glGetUniformLocation(m_uiProgramObject, "image");

    m_iLocNear = glGetUniformLocation(m_uiProgramObject, "near");
    m_iLocFar = glGetUniformLocation(m_uiProgramObject, "far");
    
    m_iLocEmitterPos = glGetUniformLocation(m_uiProgramObject, "emitterPos");
    m_iLocVelocity = glGetUniformLocation(m_uiProgramObject, "velocity");
    m_iLocType = glGetUniformLocation(m_uiProgramObject, "type");
    m_iLocTime = glGetUniformLocation(m_uiProgramObject, "time");
    m_iLocColor1 = glGetUniformLocation(m_uiProgramObject, "color1");
    m_iLocColor2 = glGetUniformLocation(m_uiProgramObject, "color2");
    
    return true;
}

void GLParticleEffect::prepareToRender()
{
    glUseProgram(m_uiProgramObject);

    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    glUniform1i(m_iLocTextureUnit, 0);
    
    glUniform3fv(m_iLocEmitterPos, 1, m_vEmitterPos);
    glUniform3fv(m_iLocVelocity, 1, m_vVelocity);
    glUniform1f(m_iLocType, m_fType);
    glUniform1f(m_iLocTime, m_fTime);
    glUniform4fv(m_iLocColor1, 1, m_fColor1);
    glUniform4fv(m_iLocColor2, 1, m_fColor2);
    glUniform1f(m_iLocNear, kNear);
    glUniform1f(m_iLocFar, kFar);
}
