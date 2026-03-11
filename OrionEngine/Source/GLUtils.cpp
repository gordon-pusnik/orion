//
//  GLUtils.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-30.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "GLUtils.h"
#include "Utils.h"
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include "Texture.h"
#include "Manager.h"
#include "Constants.h"
#include <cassert>

void GLUtils::printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

void GLUtils::checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error = glGetError()) {
//        assert(0);
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

//
/// \brief Generates geometry for a sphere.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list for a TRIANGLE_STRIP
/// \param numSlices The number of slices in the sphere
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         (if it is not NULL) as a GL_TRIANGLE_STRIP
//
int GLUtils::genSphere(int numSlices, float radius, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts)
{
    int i;
    int j;
    int numParallels = numSlices / 2;
    int numVertices = (numParallels + 1) * (numSlices + 1);
    int numIndices = numParallels * numSlices * 6;
    float angleStep = (2.0f * kPi) / ((float) numSlices);
    
    *numVerts = numVertices;
    
    // Allocate memory for buffers
    if (vertices != NULL)
        *vertices = new GLfloat[3 * numVertices];
    
    if (normals != NULL)
        *normals = new GLfloat[3 * numVertices];
    
    if (texCoords != NULL)
        *texCoords = new GLfloat[2 * numVertices];
    
    if (indices != NULL)
        *indices = new GLushort[numIndices];
    
    for (i = 0; i < numParallels + 1; i++) {
        for (j = 0; j < numSlices + 1; j++) {
            int vertex = (i * (numSlices + 1) + j) * 3;
            
            if (vertices) {
                (*vertices)[vertex + 0] = radius * sinf(angleStep * (float) i) *
                sinf(angleStep * (float) j);
                (*vertices)[vertex + 1] = radius * cosf ( angleStep * (float) i);
                (*vertices)[vertex + 2] = radius * sinf ( angleStep * (float) i) *
                cosf(angleStep * (float) j);
            }
            
            if (normals) {
                (*normals)[vertex + 0] = (*vertices)[vertex + 0] / radius;
                (*normals)[vertex + 1] = (*vertices)[vertex + 1] / radius;
                (*normals)[vertex + 2] = (*vertices)[vertex + 2] / radius;
            }
            
            if (texCoords) {
                int texIndex = (i * (numSlices + 1) + j) * 2;
                (*texCoords)[texIndex + 0] = (float) j / (float) numSlices;
                (*texCoords)[texIndex + 1] = 2 * (i + 1) / (float) numSlices;
            }
        }
    }
    
    // Generate the indices
    if (indices != NULL) {
        GLushort *indexBuf = (*indices);
        for (i = 0; i < numParallels; i++) {
            for (j = 0; j < numSlices; j++) {
                *indexBuf++ = i * (numSlices + 1) + j;
                *indexBuf++ = (i + 1) * (numSlices + 1) + j;
                *indexBuf++ = (i + 1) * (numSlices + 1) + (j + 1);
                
                *indexBuf++ = i * (numSlices + 1) + j;
                *indexBuf++ = (i + 1) * (numSlices + 1) + (j + 1);
                *indexBuf++ = i * (numSlices + 1) + (j + 1);
            }
        }
    }
    
    return numIndices;
}

int GLUtils::genSphere(int numSlices, float radius, GLfloat **vertices, GLushort **indices, int *numVerts)
{
    GLfloat *verts;
    GLfloat *normals;
    GLfloat *texCoords;
    int numVertices;
    
    int numIndices = GLUtils::genSphere(numSlices, radius, &verts, &normals, &texCoords, indices, &numVertices);
    
    *numVerts = numVertices;
    
    GLfloat *vertexes = new GLfloat[8 * numVertices];
    
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    
    for (int vert = 0; vert < numVertices; vert++) {
        vertexes[i++] = verts[j++];
        vertexes[i++] = verts[j++];
        vertexes[i++] = verts[j++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = texCoords[l++];
        vertexes[i++] = texCoords[l++];
    }
    
    delete[] verts;
    delete[] normals;
    delete[] texCoords;
    
    *vertices = vertexes;
    
    return numIndices;    
}


//
/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list for a TRIANGLES
/// \param scale The size of the cube, use 1.0 for a unit cube.
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
int GLUtils::genCube(float scale, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts)
{
    int i;
    int numVertices = *numVerts = 24;
    int numIndices = 36;
    
    GLfloat cubeVerts[] =
    {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
        0.5f,  0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
    };
    
    GLfloat cubeNormals[] =
    {
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };
    
    GLfloat cubeTex[] =
    {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
    };
    
    // Allocate memory for buffers
    if (vertices != NULL) {
        *vertices = new GLfloat[3 * numVertices];
        memcpy(*vertices, cubeVerts, sizeof(cubeVerts));
        for (i = 0; i < numVertices * 3; i++) {
            (*vertices)[i] *= scale;
        }
    }
    
    if (normals != NULL) {
        *normals = new GLfloat[3 * numVertices];
        memcpy(*normals, cubeNormals, sizeof(cubeNormals));
    }
    
    if (texCoords != NULL) {
        *texCoords = new GLfloat[2 * numVertices];
        memcpy(*texCoords, cubeTex, sizeof(cubeTex)) ;
    }
    
    // Generate the indices
    if (indices != NULL) {
        GLushort cubeIndices[] =
        {
            0, 2, 1,
            0, 3, 2,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            12, 15, 14,
            12, 14, 13,
            16, 17, 18,
            16, 18, 19,
            20, 23, 22,
            20, 22, 21
        };
        
        *indices = new GLushort[numIndices];
        memcpy(*indices, cubeIndices, sizeof(cubeIndices));
    }
    
    return numIndices;
}

// Returns an interleaved cube, aka array of structures
int GLUtils::genCube(float scale, GLfloat **vertices, GLushort **indices, int *numVerts)
{
    GLfloat *verts;
    GLfloat *normals;
    GLfloat *texCoords;
    GLushort *indexes;
    int numVertices;
    
    int numIndices = GLUtils::genCube(scale, &verts, &normals, &texCoords, &indexes, &numVertices);
    
    *numVerts = numVertices;
    
    GLfloat *vertexes = new GLfloat[8 * numVertices];
    
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    
    for (int vert = 0; vert < numVertices; vert++) {
        vertexes[i++] = verts[j++];
        vertexes[i++] = verts[j++];
        vertexes[i++] = verts[j++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = texCoords[l++];
        vertexes[i++] = texCoords[l++];
    }
    
    delete[] verts;
    delete[] normals;
    delete[] texCoords;
    
    *vertices = vertexes;
    *indices = indexes;
    
    return numIndices;
}

int GLUtils::genSquare(float scale, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts)
{
    int i;
    int numVertices = *numVerts = 4;
    int numIndices = 6;
    
    GLfloat squareVerts[] =
    {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f,  0.0f,
        0.5f, 0.5f,  0.0f,
        0.5f, -0.5f, 0.0f
    };
    
    GLfloat squareNormals[] =
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };
    
    GLfloat squareTex[] =
    {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };
    
    // Allocate memory for buffers
    if (vertices != NULL) {
        *vertices = new GLfloat[3 * numVertices];
        memcpy(*vertices, squareVerts, sizeof(squareVerts));
        for (i = 0; i < numVertices * 3; i++) {
            (*vertices)[i] *= scale;
        }
    }
    
    if (normals != NULL) {
        *normals = new GLfloat[3 * numVertices];
        memcpy(*normals, squareNormals, sizeof(squareNormals));
    }
    
    if (texCoords != NULL) {
        *texCoords = new GLfloat[2 * numVertices];
        memcpy(*texCoords, squareTex, sizeof(squareTex)) ;
    }
    
    // Generate the indices
    if (indices != NULL) {
        GLushort squareIndices[] =
        {
            0, 2, 1,
            0, 3, 2
        };
        
        *indices = new GLushort[numIndices];
        memcpy(*indices, squareIndices, sizeof(squareIndices));
    }
    
    return numIndices;    
}

int GLUtils::genSquare(float scale, GLfloat **vertices, GLushort **indices, int *numVerts)
{
    GLfloat *verts;
    GLfloat *normals;
    GLfloat *texCoords;
    GLushort *indexes;
    int numVertices;
    
    int numIndices = GLUtils::genSquare(scale, &verts, &normals, &texCoords, &indexes, &numVertices);
    
    *numVerts = numVertices;
    
    GLfloat *vertexes = new GLfloat[8 * numVertices];
    
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    
    for (int vert = 0; vert < numVertices; vert++) {
        vertexes[i++] = verts[j++];
        vertexes[i++] = verts[j++];
        vertexes[i++] = verts[j++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = texCoords[l++];
        vertexes[i++] = texCoords[l++];
    }
    
    delete[] verts;
    delete[] normals;
    delete[] texCoords;
    
    *vertices = vertexes;
    *indices = indexes;
    
    return numIndices;    
}

int GLUtils::genRect(float scalew, float scaleh, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts)
{
    int i;
    int numVertices = *numVerts = 4;
    int numIndices = 6;
    
    GLfloat squareVerts[] =
    {
        -0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f,  0.0f,
        0.5f, 0.5f,  0.0f,
        0.5f, -0.5f, 0.0f
    };
    
    GLfloat squareNormals[] =
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };
    
    GLfloat squareTex[] =
    {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
    };
    
    // Allocate memory for buffers
    if (vertices != NULL) {
        *vertices = new GLfloat[3 * numVertices];
        memcpy(*vertices, squareVerts, sizeof(squareVerts));
        for (i = 0; i < numVertices * 3; i += 3) {
            (*vertices)[i + 0] *= scalew;
            (*vertices)[i + 1] *= scaleh;
            (*vertices)[i + 2] *= scalew;
        }
    }
    
    if (normals != NULL) {
        *normals = new GLfloat[3 * numVertices];
        memcpy(*normals, squareNormals, sizeof(squareNormals));
    }
    
    if (texCoords != NULL) {
        *texCoords = new GLfloat[2 * numVertices];
        memcpy(*texCoords, squareTex, sizeof(squareTex)) ;
    }
    
    // Generate the indices
    if (indices != NULL) {
        GLushort squareIndices[] =
        {
            0, 2, 1,
            0, 3, 2
        };
        
        *indices = new GLushort[numIndices];
        memcpy(*indices, squareIndices, sizeof(squareIndices));
    }
    
    return numIndices;
}

int GLUtils::genRect(float scalew, float scaleh, GLfloat **vertices, GLushort **indices, int *numVerts)
{
    GLfloat *verts;
    GLfloat *normals;
    GLfloat *texCoords;
    GLushort *indexes;
    int numVertices;
    
    int numIndices = GLUtils::genRect(scalew, scaleh, &verts, &normals, &texCoords, &indexes, &numVertices);
    
    *numVerts = numVertices;
    
    GLfloat *vertexes = new GLfloat[8 * numVertices];
    
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    
    for (int vert = 0; vert < numVertices; vert++) {
        vertexes[i++] = verts[j++];
        vertexes[i++] = verts[j++];
        vertexes[i++] = verts[j++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = texCoords[l++];
        vertexes[i++] = texCoords[l++];
    }
    
    delete[] verts;
    delete[] normals;
    delete[] texCoords;
    
    *vertices = vertexes;
    *indices = indexes;
    
    return numIndices;
}

int GLUtils::genTorus(float r1, float r2, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts)
{
    int num_segments = 30;
    int num_rings = 100;
    int num_vertices = (num_rings) * (num_segments);
    int num_indices = num_vertices * 2;
    
    *numVerts = num_vertices;
    
    // Allocate memory for buffers
    if (vertices != NULL)
        *vertices = new GLfloat[3 * num_vertices];
    
    if (normals != NULL)
        *normals = new GLfloat[3 * num_vertices];
    
    if (texCoords != NULL)
        *texCoords = new GLfloat[2 * num_vertices];
    
    if (indices != NULL)
        *indices = new GLushort[num_indices];
    
    for (int i = 0, index = 0, t_index = 0; i < num_rings; i++) {
        for (int j = 0; j < num_segments; j++, index += 3, t_index += 2) {
            // Compute texture coordinates (surface parameters)
            float u = float(i) / num_rings;
            float v = float(j) / num_segments;
            
            // Compute angles
            float u_angle = u * 2 * kPi;
            float v_angle = v * 2 * kPi;
            
            // Position
            float x = cos(u_angle) * (r1 + cos(v_angle) * r2);
            float y = sin(u_angle) * (r1 + cos(v_angle) * r2);
            float z = sin(v_angle) * r2;
            
            // Normal vector
            float nx = cos(u_angle) * cos(v_angle);
            float ny = sin(u_angle) * cos(v_angle);
            float nz = sin(v_angle);
                        
            (*vertices)[index + 0] = x;
            (*vertices)[index + 1] = y;
            (*vertices)[index + 2] = z;
            
            (*normals)[index + 0] = nx;
            (*normals)[index + 1] = ny;
            (*normals)[index + 2] = nz;
            
            (*texCoords)[t_index + 0] = u;
            (*texCoords)[t_index + 1] = v;
        }
    }
    
    // Compute torus indices
    GLushort *indexBuf = (*indices);
    for(int i = 0; i < num_vertices; i++) {
        *indexBuf++ = int (i % num_vertices);
        *indexBuf++ = int ((i + num_segments + 1) % num_vertices);
    }
    
    return num_indices;
}

int GLUtils::genTorus(float r1, float r2, GLfloat **vertices, GLushort **indices, int *numVerts)
{
    GLfloat *verts;
    GLfloat *normals;
    GLfloat *texCoords;
    GLushort *indexes;
    int numVertices;
    
    int numIndices = GLUtils::genTorus(r1, r2, &verts, &normals, &texCoords, &indexes, &numVertices);
    
    *numVerts = numVertices;
    
    GLfloat *vertexes = new GLfloat[8 * numVertices];
    
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    
    for (int vert = 0; vert < numVertices; vert++) {
        vertexes[i++] = verts[j++];
        vertexes[i++] = verts[j++];
        vertexes[i++] = verts[j++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = normals[k++];
        vertexes[i++] = texCoords[l++];
        vertexes[i++] = texCoords[l++];
    }
    
    delete[] verts;
    delete[] normals;
    delete[] texCoords;
    
    *vertices = vertexes;
    *indices = indexes;
    
    return numIndices;
}

// Load a PNG as a 2D Texture. Completely initialize the state
bool GLUtils::loadPNGTexture(const char *fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
    
    Texture *tex = new Texture(fileName);

    if (tex->mVBitMap == NULL) {
        delete tex;
        return false;
    }

    pBits = (GLbyte*)tex->mVBitMap->pixels;

    nWidth = tex->mVBitMap->width;
    nHeight = tex->mVBitMap->height;
    nComponents = GL_RGBA;
    eFormat = GL_RGBA;
	
	if (pBits == NULL) {
        delete tex;
		return false;
    }
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
				 eFormat, GL_UNSIGNED_BYTE, pBits);
        
    if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
        minFilter == GL_LINEAR_MIPMAP_NEAREST ||
        minFilter == GL_NEAREST_MIPMAP_LINEAR ||
        minFilter == GL_NEAREST_MIPMAP_NEAREST)
        glGenerateMipmap(GL_TEXTURE_2D);
    
    delete tex;
    
	return true;
}

// Load a TGA as a 2D Texture. Completely initialize the state
bool GLUtils::loadTGATexture(const char *fileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte *pBits;
	int nWidth, nHeight, nComponents;
	GLenum eFormat;
	
	// Read the texture bits
	pBits = GLUtils::readTGABits(fileName, &nWidth, &nHeight, &nComponents, &eFormat);
	if (pBits == NULL)
		return false;
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0,
				 eFormat, GL_UNSIGNED_BYTE, pBits);
    
    free(pBits);
    
    if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
       minFilter == GL_LINEAR_MIPMAP_NEAREST ||
       minFilter == GL_NEAREST_MIPMAP_LINEAR ||
       minFilter == GL_NEAREST_MIPMAP_NEAREST)
        glGenerateMipmap(GL_TEXTURE_2D);
    
	return true;
}

// Define targa header. This is only used locally.
#pragma pack(1)
typedef struct
{
    GLbyte	identsize;              // Size of ID field that follows header (0)
    GLbyte	colorMapType;           // 0 = None, 1 = paletted
    GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short	colorMapStart;          // First colour map entry
    unsigned short	colorMapLength;         // Number of colors
    unsigned char 	colorMapBits;   // bits per palette entry
    unsigned short	xstart;                 // image x origin
    unsigned short	ystart;                 // image y origin
    unsigned short	width;                  // width in pixels
    unsigned short	height;                 // height in pixels
    GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte	descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)

GLbyte *GLUtils::readTGABits(const char *fileName, GLint *width, GLint *height, GLint *components, GLenum *format, GLbyte *pData)
{
    TGAHEADER tgaHeader;		// TGA file header
    unsigned long lImageSize;		// Size in bytes of image
    short sDepth;			// Pixel depth;
    GLbyte	*pBits = NULL;          // Pointer to bits
    
    // Default/Failed values
    *width = 0;
    *height = 0;
    *format = GL_RGB;
    *components = GL_RGB;

#ifdef __APPLE__
    // Attempt to open the file
    FILE *pFile = fopen(fileName, "rb");
    if (pFile == NULL)
        return NULL;
	
    // Read in header (binary)
    fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);
#elif WIN32
	// Attempt to open the file
    FILE *pFile = fopen(fileName, "rb");
    if (pFile == NULL)
        return NULL;
	
    // Read in header (binary)
    fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);
#else
    AAsset *asset = AAssetManager_open(Manager::assetManager()->getAAssetManager(), fileName, 0);
    if (asset == NULL) {
    	LOGI("Could not open asset\n");
    	return NULL;
    }

    AAsset_read(asset, &tgaHeader, 18);
#endif
    // Do byte swap for big vs little endian

    // Get width, height, and depth of texture
    *width = tgaHeader.width;
    *height = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;
    
    // Put some validity checks here. Very simply, I only understand
    // or care about 8, 24, or 32 bit targa's.
    if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;
	
    // Calculate size of image buffer
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    
    // Allocate memory and check for success
    if(pData == NULL)
        pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    else
        pBits = pData;
    
    // Read in the bits
    // Check for read error. This should catch RLE or other
    // weird formats that I don't want to recognize
#ifdef __APPLE__
    if (fread(pBits, lImageSize, 1, pFile) != 1)
    {
        if (pBits != NULL)
            free(pBits);
        return NULL;
    }
#elif WIN32
    if (fread(pBits, lImageSize, 1, pFile) != 1)
    {
        if (pBits != NULL)
            free(pBits);
        return NULL;
    }
#else
    if (AAsset_read(asset, pBits, lImageSize) < 0) {
        if (pBits != NULL)
            free(pBits);
        return NULL;
    }
#endif
    
    // Set OpenGL format expected
    switch(sDepth)
    {
        case 3:     // Most likely case
            *format = GL_RGB;
            *components = GL_RGB;
            for(int i = 0; i < lImageSize; i += 3) {
                GLbyte temp = pBits[i];
                pBits[i] = pBits[i+2];
                pBits[i+2] = temp;
            }
            break;
        case 4:
            *format = GL_RGBA;
            *components = GL_RGBA;
            for(int i = 0; i < lImageSize; i += 4) {
                 GLbyte temp = pBits[i];
                 pBits[i] = pBits[i+2];
                 pBits[i+2] = temp;
            }
            break;
        case 1:
//            *format = GL_LUMINANCE;
//            *components = GL_LUMINANCE;
            break;
        default:        // RGB
            break;
    }
	
    // Done with File
#ifdef __APPLE__
    fclose(pFile);
#elif WIN32
	fclose(pFile);
#else
    AAsset_close(asset);
#endif
    
    // Flip the image
    GLbyte* pFlippedBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
    GLUtils::flipBitmap(pBits, pFlippedBits, *width, *height, sDepth);
	free(pBits);
    pBits = pFlippedBits;
    
    // Return pointer to image data
    return pBits;
}

void GLUtils::flipBitmap(GLbyte *srcPixels, GLbyte *dstPixels, int width, int height, int depth)
{
	int row = height - 1;
	int srcIndex = 0;
	int dstIndex = 0;
	int width_xDepth = width * depth;
    
	for (int y = 0; y < height; y++) {
		dstIndex = width_xDepth * row;
		row--;
		
		memcpy(&dstPixels[srcIndex], &srcPixels[dstIndex], width_xDepth);
        
		srcIndex += width_xDepth;
	}
}

