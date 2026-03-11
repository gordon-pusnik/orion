//
//  ObjLoader.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-17.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "ObjLoader.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include "Manager.h"
#include "Utils.h"
#include "ModelOBJ.h"

struct coord
{
    float x, y, z;
    coord(float a, float b, float c) : x(a), y(b), z(c) {};
};

ObjLoader::ObjLoader()
{
    
}

ObjLoader::~ObjLoader()
{
    
}

// Returns an interleaved model, aka array of structures
int ObjLoader::loadModel(const char* file, GLfloat **vertices, GLushort **indices, int *numVerts, char* texFileName, float *radius, float scale)
{
    GLfloat *verts;
    GLfloat *normals;
    GLfloat *texCoords;
    GLushort *indexes;
    int numVertices;
    
    int numIndices = loadModel(file, &verts, &normals, &texCoords, &indexes, &numVertices, texFileName, radius, scale);
    
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

int ObjLoader::loadModel(const char* file, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts, char* texFileName, float *radius, float scale)
{
    ModelOBJ m_obj;

    if (! m_obj.import(file)) {
        return 0;
    }
    
    int numMeshes = m_obj.getNumberOfMeshes();
    
    const ModelOBJ::Mesh *pMesh = &m_obj.getMesh(0);
    
    int triCount = pMesh->triangleCount;
    
    int vertCount = m_obj.getNumberOfVertices();
    
    const ModelOBJ::Material *pMaterial = &m_obj.getMaterial(0);
    
    LOGI("Num meshes: %d, vertexes: %d, numTris: %d, texture: %s\n", numMeshes, vertCount, triCount, pMaterial->colorMapFilename.c_str());
    
    strcpy(texFileName, pMaterial->colorMapFilename.c_str());
    
    *numVerts = vertCount;
    
    if (vertices != NULL) {
        *vertices = new GLfloat[3 * *numVerts];
    }

    if (normals != NULL) {
        *normals = new GLfloat[3 * *numVerts];
    }

    if (texCoords != NULL) {
        *texCoords = new GLfloat[2 * *numVerts];
    }
    
    const ModelOBJ::Vertex* vertBuffer = m_obj.getVertexBuffer();
    
    int j = 0;
    int k = 0;
    int l = 0;
    for (int i = 0; i < vertCount; i++) {
        ModelOBJ::Vertex v = vertBuffer[i];
        (*vertices)[j++] = v.position[0] * scale;
        (*vertices)[j++] = v.position[1] * scale;
        (*vertices)[j++] = v.position[2] * scale;
        (*normals)[k++] = v.normal[0];
        (*normals)[k++] = v.normal[1];
        (*normals)[k++] = v.normal[2];
        (*texCoords)[l++] = v.texCoord[0];
        (*texCoords)[l++] = v.texCoord[1];
    }

    int numIndices = triCount * 3;
    const int *ind = m_obj.getIndexBuffer() + pMesh->startIndex;
    
    if (indices != NULL) {
        *indices = new GLushort[numIndices];
        for (int i = 0; i < numIndices; i++) {
            (*indices)[i] = ind[i];
        }
    }
    
    *radius = m_obj.getRadius() * scale;
    
    return numIndices;
}
