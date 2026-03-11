//
//  GLPointSpriteStarEffect2.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-03.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "GLPointSpriteStarEffect2.h"
#include "ShaderUtils.h"
#include "Utils.h"
#include <cstdlib>
#include "Constants.h"
#include "AssetManager.h"
#include <string>


GLPointSpriteStarEffect2::GLPointSpriteStarEffect2()
{
    m_fRelativeSize = 1.0;
    m_fScale = 1.0;
    m_fSelectedIndex = -1.0;
}

GLPointSpriteStarEffect2::~GLPointSpriteStarEffect2()
{
    glDeleteProgram(m_uiProgramObject);
}

bool GLPointSpriteStarEffect2::init()
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
    
    m_uiProgramObject = ShaderUtils::createProgramWithAttributes("Shaders/PointSpriteStarEffect2.glsl",
                                                                 &vert,
                                                                 &frag,
                                                                 4,
                                                                 VertexAttribVertex, "vVertex",
                                                                 VertexAttribPointSize, "vPointSize",
                                                                 VertexAttribColor, "vColor",
                                                                 VertexAttribIndex, "vIndex");
    if (m_uiProgramObject == 0)
        return false;

    // Bind uniforms
    m_iLocTransformP = glGetUniformLocation(m_uiProgramObject, "pMatrix");
    m_iLocTransformM = glGetUniformLocation(m_uiProgramObject, "mvMatrix");
    m_iLocTextureUnit = glGetUniformLocation(m_uiProgramObject, "image");

    m_iLocNear = glGetUniformLocation(m_uiProgramObject, "near");
    m_iLocFar = glGetUniformLocation(m_uiProgramObject, "far");
    
    m_iLocRelativeSize = glGetUniformLocation(m_uiProgramObject, "relativeSize");
    m_iLocScale = glGetUniformLocation(m_uiProgramObject, "scale");
    m_iLocSelectedIndex = glGetUniformLocation(m_uiProgramObject, "selectedIndex");
    
    return true;
}

void GLPointSpriteStarEffect2::prepareToRender()
{
    glUseProgram(m_uiProgramObject);

    glUniformMatrix4fv(m_iLocTransformP, 1, GL_FALSE, m_transform.projectionMatrix.m);
    glUniformMatrix4fv(m_iLocTransformM, 1, GL_FALSE, m_transform.modelViewMatrix.m);
    glUniform1i(m_iLocTextureUnit, 0);
    glUniform1f(m_iLocRelativeSize, m_fRelativeSize);
    glUniform1f(m_iLocScale, m_fScale);
    glUniform1f(m_iLocSelectedIndex, m_fSelectedIndex);
    glUniform1f(m_iLocNear, kNear);
    glUniform1f(m_iLocFar, kFar);
}
