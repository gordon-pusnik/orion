//
//  GLBatch.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-11.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GLBatch.h"
#include "GLBaseEffect.h"
#include "GLUtils.h"

#ifdef __APPLE__
#ifdef OPENGL_ES
#import <OpenGLES/ES2/glext.h>
#endif
#endif

#ifdef __APPLE__
#ifndef OPENGL_ES
#include <OpenGL/gl.h>
#endif
#endif

#define glGenVertexArrays glGenVertexArraysAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE

GLBatch::GLBatch()
{
    m_nNumTextureUnits = 0;
    m_nNumVerts = 0;
    m_nNumIndices = 0;
    m_uiVertexArray = 0;
    m_uiNormalArray = 0;
    m_uiColorArray = 0;
    m_uiIndexArray = 0;
    m_bBatchDone = false;
    m_nVertsBuilding = 0;
    m_uiTextureCoordArray = NULL;
}

GLBatch::~GLBatch()
{
	// Delete vertex buffer objects
	if (m_uiVertexArray != 0) {
		glDeleteBuffers(1, &m_uiVertexArray);
    }
	
	if (m_uiNormalArray != 0) {
		glDeleteBuffers(1, &m_uiNormalArray);
    }
	
	if (m_uiColorArray != 0) {
		glDeleteBuffers(1, &m_uiColorArray);
    }
    
    if (m_uiIndexArray != 0) {
        glDeleteBuffers(1, &m_uiIndexArray);
    }
	
	for (unsigned int i = 0; i < m_nNumTextureUnits; i++) {
		glDeleteBuffers(1, &m_uiTextureCoordArray[i]);
    }
    
	delete[] m_uiTextureCoordArray;
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glDeleteVertexArraysOES(1, &m_uiVaoID);
#else
    glDeleteVertexArrays(1, &m_uiVaoID);
#endif
#endif
}

// Start the primitive batch.
void GLBatch::begin(GLenum primitive, GLuint nVerts, GLuint nTextureUnits)
{
	m_ePrimitiveType = primitive;
	m_nNumVerts = nVerts;
    
    if (nTextureUnits > 4)  { // Limit to four texture units
        nTextureUnits = 4;
    }
    
	m_nNumTextureUnits = nTextureUnits;
	
	if (m_nNumTextureUnits != 0) {
		m_uiTextureCoordArray = new GLuint[m_nNumTextureUnits];
        
		// An array of pointers to texture coordinate arrays
		for (unsigned int i = 0; i < m_nNumTextureUnits; i++) {
			m_uiTextureCoordArray[i] = 0;
        }
    }
}

// Start the primitive batch.
void GLBatch::beginInterleaved(GLenum primitive, GLuint nVerts, GLuint nTextureUnits)
{
    begin(primitive, nVerts, nTextureUnits);	
}

void GLBatch::copyIndices(GLushort *indices, GLuint numIndices)
{
    m_nNumIndices = numIndices;
    
    glGenBuffers(1, &m_uiIndexArray);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numIndices, indices, GL_STATIC_DRAW);
}

// Vertex (3), normal (3), texture coords (2)
void GLBatch::copyInterleavedVertexData(GLfloat *vVerts)
{
	// First time, create the buffer object, allocate the space
	if (m_uiVertexArray == 0) {
        glGenBuffers(1, &m_uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8 * m_nNumVerts, vVerts, GL_STATIC_DRAW);
    }
    else {
        // Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 8 * m_nNumVerts, vVerts);
    }    
}

// Vertex (3), normal (3)
void GLBatch::copyInterleavedVertexNormalData(GLfloat *vVerts)
{
	// First time, create the buffer object, allocate the space
	if (m_uiVertexArray == 0) {
        glGenBuffers(1, &m_uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * m_nNumVerts, vVerts, GL_STATIC_DRAW);
    }
    else {
        // Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 6 * m_nNumVerts, vVerts);
    }
}

// Vertex (3), texture coords (2)
void GLBatch::copyInterleavedVertexTexCoordData(GLfloat *vVerts)
{
	// First time, create the buffer object, allocate the space
	if (m_uiVertexArray == 0) {
        glGenBuffers(1, &m_uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 5 * m_nNumVerts, vVerts, GL_STATIC_DRAW);
    }
    else {
        // Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 5 * m_nNumVerts, vVerts);
    }
}

// Vertex (3), color (4)
void GLBatch::copyInterleavedVertexColorData(GLfloat *vVerts)
{
	// First time, create the buffer object, allocate the space
	if (m_uiVertexArray == 0) {
        glGenBuffers(1, &m_uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 7 * m_nNumVerts, vVerts, GL_STATIC_DRAW);
    }
    else {
        // Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 7 * m_nNumVerts, vVerts);
    }
}

// Vertex (3), point size (1), color (4), texture (2)
void GLBatch::copyInterleavedVertexSizeColorTexCoordData(GLfloat *vVerts)
{
	// First time, create the buffer object, allocate the space
	if (m_uiVertexArray == 0) {
        glGenBuffers(1, &m_uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 10 * m_nNumVerts, vVerts, GL_STATIC_DRAW);
    }
    else {
        // Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 10 * m_nNumVerts, vVerts);
    }    
}

// Vertex (3), point size (1), color (4), index (1)
void GLBatch::copyInterleavedVertexSizeColorIndexData(GLfloat *vVerts)
{
	// First time, create the buffer object, allocate the space
	if (m_uiVertexArray == 0) {
        glGenBuffers(1, &m_uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 9 * m_nNumVerts, vVerts, GL_STATIC_DRAW);
    }
    else {
        // Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 9 * m_nNumVerts, vVerts);
    }
}

// Velocity (3), time (1)
void GLBatch::copyInterleavedVelocityTimeData(GLfloat *vVerts)
{
	// First time, create the buffer object, allocate the space
	if (m_uiVertexArray == 0) {
        glGenBuffers(1, &m_uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * m_nNumVerts, vVerts, GL_STATIC_DRAW);
    }
    else {
        // Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * m_nNumVerts, vVerts);
    }
}

// Block Copy in vertex data
void GLBatch::copyVertexData3f(Vector3f *vVerts)
{
	// First time, create the buffer object, allocate the space
	if (m_uiVertexArray == 0) {
        glGenBuffers(1, &m_uiVertexArray);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_nNumVerts, vVerts, GL_STATIC_DRAW);
    }
    else {
        // Just bind to existing object
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * m_nNumVerts, vVerts);
    }
}

// Block copy in normal data
void GLBatch::copyNormalData3f(Vector3f *vNorms)
{
	// First time, create the buffer object, allocate the space
	if (m_uiNormalArray == 0) {
		glGenBuffers(1, &m_uiNormalArray);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * m_nNumVerts, vNorms, GL_STATIC_DRAW);
    }
	else {
        // Just bind to existing object
		glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * m_nNumVerts, vNorms);
    }
}

void GLBatch::copyColorData4f(Vector4f *vColors)
{
	// First time, create the buffer object, allocate the space
	if (m_uiColorArray == 0) {
		glGenBuffers(1, &m_uiColorArray);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * m_nNumVerts, vColors, GL_STATIC_DRAW);
	}
	else {
        // Just bind to existing object
		glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 4 * m_nNumVerts, vColors);
    }
}

void GLBatch::copyTexCoordData2f(Vector2f *vTexCoords, GLuint uiTextureLayer)
{
	// First time, create the buffer object, allocate the space
	if (m_uiTextureCoordArray[uiTextureLayer] == 0) {
		glGenBuffers(1, &m_uiTextureCoordArray[uiTextureLayer]);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[uiTextureLayer]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * m_nNumVerts, vTexCoords, GL_STATIC_DRAW);
    }
	else {
        // Just bind to existing object
		glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[uiTextureLayer]);
        
        // Copy the data in
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 2 * m_nNumVerts, vTexCoords);
    }
}

// Bind everything up in a little package
void GLBatch::end()
{
#ifdef __APPLE__
#ifdef OPENGL_ES
    glGenVertexArraysOES(1, &m_uiVaoID);
    glBindVertexArrayOES(m_uiVaoID);
#else
    glGenVertexArrays(1, &m_uiVaoID);
    glBindVertexArray(m_uiVaoID);
#endif
#endif
    
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
	if (m_uiColorArray != 0) {
		glEnableVertexAttribArray(VertexAttribColor);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
		glVertexAttribPointer(VertexAttribColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
	if (m_uiNormalArray != 0) {
		glEnableVertexAttribArray(VertexAttribNormal);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
		glVertexAttribPointer(VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
	// How many texture units
	for (unsigned int i = 0; i < m_nNumTextureUnits; i++) {
		if (m_uiTextureCoordArray[i] != 0) {
			glEnableVertexAttribArray(VertexAttribTexCoord0 + i),
			glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[i]);
			glVertexAttribPointer(VertexAttribTexCoord0 + i, 2, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#endif
	
	m_bBatchDone = true;
}

void GLBatch::endInterleaved()
{
#ifdef __APPLE__
#ifdef OPENGL_ES
    glGenVertexArraysOES(1, &m_uiVaoID);
    glBindVertexArrayOES(m_uiVaoID);
#else
    glGenVertexArrays(1, &m_uiVaoID);
    glBindVertexArray(m_uiVaoID);
#endif
#endif
    
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
		glEnableVertexAttribArray(VertexAttribNormal);
		glEnableVertexAttribArray(VertexAttribTexCoord0);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
        glVertexAttribPointer(VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#endif
	
	m_bBatchDone = true;    
}

void GLBatch::endInterleavedVertexNormal()
{
#ifdef __APPLE__
#ifdef OPENGL_ES
    glGenVertexArraysOES(1, &m_uiVaoID);
    glBindVertexArrayOES(m_uiVaoID);
#else
    glGenVertexArrays(1, &m_uiVaoID);
    glBindVertexArray(m_uiVaoID);
#endif
#endif
    
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
		glEnableVertexAttribArray(VertexAttribNormal);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#endif
	
	m_bBatchDone = true;
}

void GLBatch::endInterleavedVertexTexCoord()
{
#ifdef __APPLE__
#ifdef OPENGL_ES
    glGenVertexArraysOES(1, &m_uiVaoID);
    glBindVertexArrayOES(m_uiVaoID);
#else
    glGenVertexArrays(1, &m_uiVaoID);
    glBindVertexArray(m_uiVaoID);
#endif
#endif
    
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
		glEnableVertexAttribArray(VertexAttribTexCoord0);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
        glVertexAttribPointer(VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#endif
	
	m_bBatchDone = true;
}

void GLBatch::endInterleavedColor()
{
#ifdef __APPLE__
#ifdef OPENGL_ES
    glGenVertexArraysOES(1, &m_uiVaoID);
    glBindVertexArrayOES(m_uiVaoID);
#else
    glGenVertexArrays(1, &m_uiVaoID);
    glBindVertexArray(m_uiVaoID);
#endif
#endif
    
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
		glEnableVertexAttribArray(VertexAttribColor);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribColor, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#endif
	
	m_bBatchDone = true;
}

void GLBatch::endInterleavedVertexSizeColorTexCoord()
{
#ifdef __APPLE__
#ifdef OPENGL_ES
    glGenVertexArraysOES(1, &m_uiVaoID);
    glBindVertexArrayOES(m_uiVaoID);
#else
    glGenVertexArrays(1, &m_uiVaoID);
    glBindVertexArray(m_uiVaoID);
#endif
#endif
    
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
        glEnableVertexAttribArray(VertexAttribPointSize);
		glEnableVertexAttribArray(VertexAttribColor);
		glEnableVertexAttribArray(VertexAttribTexCoord0);        
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribPointSize, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const void*) offset);
        offset += 1 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribColor, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const void*) offset);
        offset += 4 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#endif
	
	m_bBatchDone = true;
}

void GLBatch::endInterleavedVertexSizeColorIndex()
{
#ifdef __APPLE__
#ifdef OPENGL_ES
    glGenVertexArraysOES(1, &m_uiVaoID);
    glBindVertexArrayOES(m_uiVaoID);
#else
    glGenVertexArrays(1, &m_uiVaoID);
    glBindVertexArray(m_uiVaoID);
#endif
#endif
    
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
        glEnableVertexAttribArray(VertexAttribPointSize);
		glEnableVertexAttribArray(VertexAttribColor);
		glEnableVertexAttribArray(VertexAttribIndex);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribPointSize, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const void*) offset);
        offset += 1 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribColor, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const void*) offset);
        offset += 4 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribIndex, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#endif
	
	m_bBatchDone = true;
}

void GLBatch::endInterleavedVelocityTime()
{
#ifdef __APPLE__
#ifdef OPENGL_ES
    glGenVertexArraysOES(1, &m_uiVaoID);
    glBindVertexArrayOES(m_uiVaoID);
#else
    glGenVertexArrays(1, &m_uiVaoID);
    glBindVertexArray(m_uiVaoID);
#endif
#endif
    
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
        glEnableVertexAttribArray(VertexAttribTime);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribTime, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#endif
	
	m_bBatchDone = true;
}

// Just start over. No reallocations, etc.
void GLBatch::reset(void)
{
	m_bBatchDone = false;
	m_nVertsBuilding = 0;
}

void GLBatch::render(void)
{
	if (! m_bBatchDone) {
		return;
    }

#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(m_uiVaoID);
#else
    glBindVertexArray(m_uiVaoID);
#endif
#else // No vaos - android
    if (m_uiVertexArray != 0) {
        glEnableVertexAttribArray(VertexAttribVertex);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
    if (m_uiColorArray != 0) {
        glEnableVertexAttribArray(VertexAttribColor);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiColorArray);
        glVertexAttribPointer(VertexAttribColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
    if (m_uiNormalArray != 0) {
        glEnableVertexAttribArray(VertexAttribNormal);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiNormalArray);
        glVertexAttribPointer(VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
    // How many texture units
    for (unsigned int i = 0; i < m_nNumTextureUnits; i++) {
        if (m_uiTextureCoordArray[i] != 0) {
            glEnableVertexAttribArray(VertexAttribTexCoord0 + i),
            glBindBuffer(GL_ARRAY_BUFFER, m_uiTextureCoordArray[i]);
            glVertexAttribPointer(VertexAttribTexCoord0 + i, 2, GL_FLOAT, GL_FALSE, 0, 0);
        }
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
#endif
    
    if (m_uiIndexArray) {
        glDrawElements(m_ePrimitiveType, m_nNumIndices, GL_UNSIGNED_SHORT, 0);
    }
    else {
        glDrawArrays(m_ePrimitiveType, 0, m_nNumVerts);
    }

#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#else
     glDisableVertexAttribArray(VertexAttribVertex);
     glDisableVertexAttribArray(VertexAttribNormal);
     glDisableVertexAttribArray(VertexAttribColor);
     
     for (unsigned int i = 0; i < m_nNumTextureUnits; i++) {
         if (m_uiTextureCoordArray[i] != 0) {
             glDisableVertexAttribArray(VertexAttribTexCoord0 + i);
         }
     }
#endif
}

void GLBatch::renderInterleaved()
{
	if (! m_bBatchDone) {
		return;
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(m_uiVaoID);
#else
    glBindVertexArray(m_uiVaoID);
#endif
#else // No vaos - android
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
		glEnableVertexAttribArray(VertexAttribNormal);
		glEnableVertexAttribArray(VertexAttribTexCoord0);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
        glVertexAttribPointer(VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
#endif
    
    if (m_uiIndexArray) {
        glDrawElements(m_ePrimitiveType, m_nNumIndices, GL_UNSIGNED_SHORT, 0);
    }
    else {
        glDrawArrays(m_ePrimitiveType, 0, m_nNumVerts);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#else
    glDisableVertexAttribArray(VertexAttribVertex);
    glDisableVertexAttribArray(VertexAttribNormal);
    glDisableVertexAttribArray(VertexAttribTexCoord0);
#endif
}

void GLBatch::renderInterleavedVertexNormal()
{
	if (! m_bBatchDone) {
		return;
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(m_uiVaoID);
#else
    glBindVertexArray(m_uiVaoID);
#endif
#else // No vaos - android
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
		glEnableVertexAttribArray(VertexAttribNormal);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
#endif
    
    if (m_uiIndexArray) {
        glDrawElements(m_ePrimitiveType, m_nNumIndices, GL_UNSIGNED_SHORT, 0);
    }
    else {
        glDrawArrays(m_ePrimitiveType, 0, m_nNumVerts);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#else
    glDisableVertexAttribArray(VertexAttribVertex);
    glDisableVertexAttribArray(VertexAttribNormal);
#endif
}

void GLBatch::renderInterleavedVertexTexCoord()
{
	if (! m_bBatchDone) {
		return;
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(m_uiVaoID);
#else
    glBindVertexArray(m_uiVaoID);
#endif
#else // No vaos - android
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
		glEnableVertexAttribArray(VertexAttribTexCoord0);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
        glVertexAttribPointer(VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
#endif
    
    if (m_uiIndexArray) {
        glDrawElements(m_ePrimitiveType, m_nNumIndices, GL_UNSIGNED_SHORT, 0);
    }
    else {
        glDrawArrays(m_ePrimitiveType, 0, m_nNumVerts);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#else
    glDisableVertexAttribArray(VertexAttribVertex);
    glDisableVertexAttribArray(VertexAttribTexCoord0);
#endif
}

void GLBatch::renderInterleavedColor()
{
	if (! m_bBatchDone) {
		return;
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(m_uiVaoID);
#else
    glBindVertexArray(m_uiVaoID);
#endif
#else // No vaos - android
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
		glEnableVertexAttribArray(VertexAttribColor);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribColor, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
#endif
    
    if (m_uiIndexArray) {
        glDrawElements(m_ePrimitiveType, m_nNumIndices, GL_UNSIGNED_SHORT, 0);
    }
    else {
        glDrawArrays(m_ePrimitiveType, 0, m_nNumVerts);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#else
    glDisableVertexAttribArray(VertexAttribVertex);
    glDisableVertexAttribArray(VertexAttribColor);
#endif
}

void GLBatch::renderInterleavedVertexSizeColorTexCoord()
{
	if (! m_bBatchDone) {
		return;
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(m_uiVaoID);
#else
    glBindVertexArray(m_uiVaoID);
#endif
#else // No vaos - android
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
        glEnableVertexAttribArray(VertexAttribPointSize);
		glEnableVertexAttribArray(VertexAttribColor);
		glEnableVertexAttribArray(VertexAttribTexCoord0);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribPointSize, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const void*) offset);
        offset += 1 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribColor, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const void*) offset);
        offset += 4 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
#endif
    
    if (m_uiIndexArray) {
        glDrawElements(m_ePrimitiveType, m_nNumIndices, GL_UNSIGNED_SHORT, 0);
    }
    else {
        glDrawArrays(m_ePrimitiveType, 0, m_nNumVerts);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#else
    glDisableVertexAttribArray(VertexAttribVertex);
    glDisableVertexAttribArray(VertexAttribPointSize);
    glDisableVertexAttribArray(VertexAttribColor);
    glDisableVertexAttribArray(VertexAttribTexCoord0);
#endif
}

void GLBatch::renderInterleavedVertexSizeColorIndex()
{
	if (! m_bBatchDone) {
		return;
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(m_uiVaoID);
#else
    glBindVertexArray(m_uiVaoID);
#endif
#else // No vaos - android
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
        glEnableVertexAttribArray(VertexAttribPointSize);
		glEnableVertexAttribArray(VertexAttribColor);
		glEnableVertexAttribArray(VertexAttribIndex);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribPointSize, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const void*) offset);
        offset += 1 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribColor, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const void*) offset);
        offset += 4 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribIndex, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
#endif
    
    if (m_uiIndexArray) {
        glDrawElements(m_ePrimitiveType, m_nNumIndices, GL_UNSIGNED_SHORT, 0);
    }
    else {
        glDrawArrays(m_ePrimitiveType, 0, m_nNumVerts);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#else
    glDisableVertexAttribArray(VertexAttribVertex);
    glDisableVertexAttribArray(VertexAttribPointSize);
    glDisableVertexAttribArray(VertexAttribColor);
    glDisableVertexAttribArray(VertexAttribIndex);
#endif
}

void GLBatch::renderInterleavedVelocityTime()
{
	if (! m_bBatchDone) {
		return;
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(m_uiVaoID);
#else
    glBindVertexArray(m_uiVaoID);
#endif
#else // No vaos - android
	if (m_uiVertexArray != 0) {
		glEnableVertexAttribArray(VertexAttribVertex);
        glEnableVertexAttribArray(VertexAttribTime);
		glBindBuffer(GL_ARRAY_BUFFER, m_uiVertexArray);
        int offset = 0;
		glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*) offset);
        offset += 3 * sizeof(GLfloat);
		glVertexAttribPointer(VertexAttribTime, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const void*) offset);
    }
    
    if (m_uiIndexArray) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIndexArray);
    }
#endif
    
    if (m_uiIndexArray) {
        glDrawElements(m_ePrimitiveType, m_nNumIndices, GL_UNSIGNED_SHORT, 0);
    }
    else {
        glDrawArrays(m_ePrimitiveType, 0, m_nNumVerts);
    }
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    glBindVertexArrayOES(0);
#else
    glBindVertexArray(0);
#endif
#else
    glDisableVertexAttribArray(VertexAttribVertex);
    glDisableVertexAttribArray(VertexAttribTime);
#endif
}


