//
//  GLBatch.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-11.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__GLBatch__
#define __Orion__GLBatch__

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#elif WIN32
#include "stdafx.h"
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

class GLBatch
{
public:
    GLBatch();
    ~GLBatch();
    
    // Start populating the array
    void begin(GLenum primitive, GLuint nVerts, GLuint nTextureUnits = 0);
    void beginInterleaved(GLenum primitive, GLuint nVerts, GLuint nTextureUnits = 0);
    
    // Tell the batch you are done
    void end();
    void endInterleaved();
    void endInterleavedVertexNormal();
    void endInterleavedVertexTexCoord();
    void endInterleavedColor();
    void endInterleavedVertexSizeColorTexCoord();
    void endInterleavedVertexSizeColorIndex();
    void endInterleavedVelocityTime();
        
    // Block Copy in vertex data
    void copyVertexData3f(Vector3f *vVerts);
    void copyNormalData3f(Vector3f *vNorms);
    void copyColorData4f(Vector4f *vColors);
    void copyTexCoordData2f(Vector2f *vTexCoords, GLuint uiTextureLayer);
    
    // Just to make life easier...
    void copyVertexData3f(GLfloat *vVerts) { copyVertexData3f((Vector3f *)(vVerts)); }
    void copyNormalData3f(GLfloat *vNorms) { copyNormalData3f((Vector3f *)(vNorms)); }
    void copyColorData4f(GLfloat *vColors) { copyColorData4f((Vector4f *)(vColors)); }
    void copyTexCoordData2f(GLfloat *vTex, GLuint uiTextureLayer) { copyTexCoordData2f((Vector2f *)(vTex), uiTextureLayer); }

    void copyInterleavedVertexData(GLfloat *vVerts);
    void copyInterleavedVertexNormalData(GLfloat *vVerts);
    void copyInterleavedVertexTexCoordData(GLfloat *vVerts);
    void copyInterleavedVertexColorData(GLfloat *vVerts);
    void copyInterleavedVertexSizeColorTexCoordData(GLfloat *vVerts);
    void copyInterleavedVertexSizeColorIndexData(GLfloat *vVerts);
    void copyInterleavedVelocityTimeData(GLfloat *vVerts);
    
    void copyIndices(GLushort *indices, GLuint numIndices);
    
    void render();
    void renderInterleaved();
    void renderInterleavedVertexNormal();
    void renderInterleavedVertexTexCoord();
    void renderInterleavedColor();
    void renderInterleavedVertexSizeColorTexCoord();
    void renderInterleavedVertexSizeColorIndex();
    void renderInterleavedVelocityTime();
    
    void reset();
    
private:
    GLenum m_ePrimitiveType;         // What am I drawing....
    
    GLuint m_uiVertexArray;
    GLuint m_uiNormalArray;
    GLuint m_uiColorArray;
    GLuint *m_uiTextureCoordArray;
    GLuint m_uiIndexArray;
    
    GLuint m_nVertsBuilding;		// Building up vertexes counter (immediate mode emulator)
    GLuint m_nNumVerts;				// Number of verticies in this batch
    GLuint m_nNumTextureUnits;		// Number of texture coordinate sets
    
    bool m_bBatchDone;				// Batch has been built
 
    GLuint m_nNumIndices;
    
    GLuint m_uiVaoID;
};

#endif /* defined(__Orion__GLBatch__) */
