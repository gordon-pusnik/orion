//
//  ShaderUtils.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-30.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__ShaderUtils__
#define __Orion__ShaderUtils__

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#elif WIN32
#include "stdafx.h"
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <string>

namespace ShaderUtils
{
    GLuint loadShader(GLenum shaderType, const char* pSource);
    GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);
    GLuint createProgramWithAttributes(const char* pVertexSource, const char* pFragmentSource, ...);
    GLuint createProgramWithAttributes(const char* shaderFile, std::string *vertexDefines, std::string *fragmentDefines, ...);
};

#endif /* defined(__Orion__ShaderUtils__) */
