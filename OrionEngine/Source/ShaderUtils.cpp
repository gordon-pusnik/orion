//
//  ShaderUtils.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-30.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "ShaderUtils.h"
#include "Utils.h"
#include "GLUtils.h"
#include <cstdlib>
#include <string>
#include "Manager.h"

GLuint ShaderUtils::loadShader(GLenum shaderType, const char* pSource)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (! compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint ShaderUtils::createProgram(const char* pVertexSource, const char* pFragmentSource)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (! vertexShader) {
        return 0;
    }
    
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (! fragmentShader) {
        return 0;
    }
    
    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
                
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    
    return program;
}

GLuint ShaderUtils::createProgramWithAttributes(const char* pVertexSource, const char* pFragmentSource, ...)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (! vertexShader) {
        return 0;
    }
    
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (! fragmentShader) {
        return 0;
    }
    
    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        // Attach attributes before linking.
        // List of attributes
        va_list attributeList;
        va_start(attributeList, pFragmentSource);
        
        char *szNextArg;
        int iArgCount = va_arg(attributeList, int);	// Number of attributes
        for (int i = 0; i < iArgCount; i++) {
            int index = va_arg(attributeList, int);
            szNextArg = va_arg(attributeList, char*);
            glBindAttribLocation(program, index, szNextArg);
		}
        va_end(attributeList);
        
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    
    return program;
}

GLuint ShaderUtils::createProgramWithAttributes(const char* shaderFile, std::string *vertDefines, std::string *fragDefines, ...)
{
    GLuint program = 0;
    char *buffer = 0;
    
    AssetManager *amgr = Manager::assetManager();
#ifdef __ANDROID__
    // glsl files are compressed on Android, so we can't get a file ptr.
    amgr->fillBufferWithAsset(shaderFile, &buffer);
#else
    long lSize;
    FILE *pFile = amgr->filePtrFromAsset(shaderFile, &lSize);
    if (pFile) {
        size_t result;
                
        // allocate memory to contain the whole file:
        buffer = new char[lSize + 1];
        
        // copy the file into the buffer:
        result = fread(buffer, 1, lSize, pFile);
        buffer[lSize] = 0;
        
        // terminate
        fclose(pFile);
    }
#endif

    amgr->closeAsset();
    
    if (! buffer) {
        return 0;
    }

    std::string progSource = buffer;
    std::string vertSource = *vertDefines;
    std::string fragSource = *fragDefines;
    
    vertSource += progSource;
    fragSource += progSource;
    
//    LOGI("%s\n", vertSource.c_str());
//    LOGI("%s\n", fragSource.c_str());
    
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertSource.c_str());
    if (! vertexShader) {
        return 0;
    }
    
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragSource.c_str());
    if (! fragmentShader) {
        return 0;
    }
    
    program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        
        // Attach attributes before linking.
        // List of attributes
        va_list attributeList;
        va_start(attributeList, fragDefines);
        
        char *szNextArg;
        int iArgCount = va_arg(attributeList, int);	// Number of attributes
        for (int i = 0; i < iArgCount; i++) {
            int index = va_arg(attributeList, int);
            szNextArg = va_arg(attributeList, char*);
            glBindAttribLocation(program, index, szNextArg);
        }
        va_end(attributeList);
        
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
            
    if (buffer) {
        delete[] buffer;
    }
    
    return program;
}
