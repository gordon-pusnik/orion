//
//  ObjLoader.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-17.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__ObjLoader__
#define __Orion__ObjLoader__

#include "GLUtils.h"
#include "Vector3.h"

class ObjLoader
{
public:
    ObjLoader();
    ~ObjLoader();
    
    int loadModel(const char* file, GLfloat **vertices, GLushort **indices, int *numVerts, char* texFileName, float *radius, float scale);
    int loadModel(const char* file, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLushort **indices, int *numVerts, char *texFileName, float *radius, float scale);
};

#endif /* defined(__Orion__ObjLoader__) */
