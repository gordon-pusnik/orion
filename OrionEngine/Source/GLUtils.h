//
//  GLUtils.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-30.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__GLUtils__
#define __Orion__GLUtils__

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#elif WIN32
#include "stdafx.h"
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

namespace GLUtils
{
    void printGLString(const char *name, GLenum s);
    void checkGlError(const char* op);
    int genSphere(int numSlices, float radius, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts);
    int genSphere(int numSlices, float radius, GLfloat **vertices, GLushort **indices, int *numVerts);
    int genCube(float scale, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts);
    int genCube(float scale, GLfloat **vertices, GLushort **indices, int *numVerts);
    int genTorus(float r1, float r2, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts);
    int genTorus(float r1, float r2, GLfloat **vertices, GLushort **indices, int *numVerts);
    int genSquare(float scale, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts);
    int genSquare(float scale, GLfloat **vertices, GLushort **indices, int *numVerts);
    int genRect(float scalew, float scaleh, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts);
    int genRect(float scalew, float scaleh, GLfloat **vertices, GLushort **indices, int *numVerts);
    bool loadPNGTexture(const char *fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
    bool loadTGATexture(const char *fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode);
    GLbyte *readTGABits(const char *fileName, GLint *width, GLint *height, GLint *components, GLenum *format, GLbyte *pData = NULL);
    void flipBitmap(GLbyte *srcPixels, GLbyte *dstPixels, int width, int height, int depth);
};

#endif /* defined(__Orion__GLUtils__) */
