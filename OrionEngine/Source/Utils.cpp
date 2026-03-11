//
//  Utils.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-30.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Utils.h"
#include "Constants.h"
#include <cmath>
#include "Manager.h"
#include "Entity.h"

double Utils::getScaled(double f)
{
    return f / kGalacticScale;
}

double Utils::getScaleRestored(double f)
{
    return f * kGalacticScale;
}

bool Utils::compareEntityByDistance(Entity* e1, Entity* e2)
{
    Camera *cam = Manager::camera();
    Vector3d pos1;
    e1->getPosition(pos1);
    double dist1 = cam->distance(pos1);
    
    Vector3d pos2;
    e2->getPosition(pos2);
    double dist2 = cam->distance(pos2);
    
    if (dist2 < dist1) {
        return true;
    }
    else {
        return false;
    }
}

float frustum[6][4];

void Utils::extractFrustum()
{
    Camera *cam = Manager::camera();
    double clip[16];
    double t;
    
    Matrix4 proj = cam->projectionMatrixPerspective();
    Matrix4 modl = cam->viewMatrix();
    
    /* Combine the two matrices (multiply projection by modelview) */
    clip[ 0] = modl.m[ 0] * proj.m[ 0] + modl.m[ 1] * proj.m[ 4] + modl.m[ 2] * proj.m[ 8] + modl.m[ 3] * proj.m[12];
    clip[ 1] = modl.m[ 0] * proj.m[ 1] + modl.m[ 1] * proj.m[ 5] + modl.m[ 2] * proj.m[ 9] + modl.m[ 3] * proj.m[13];
    clip[ 2] = modl.m[ 0] * proj.m[ 2] + modl.m[ 1] * proj.m[ 6] + modl.m[ 2] * proj.m[10] + modl.m[ 3] * proj.m[14];
    clip[ 3] = modl.m[ 0] * proj.m[ 3] + modl.m[ 1] * proj.m[ 7] + modl.m[ 2] * proj.m[11] + modl.m[ 3] * proj.m[15];
    
    clip[ 4] = modl.m[ 4] * proj.m[ 0] + modl.m[ 5] * proj.m[ 4] + modl.m[ 6] * proj.m[ 8] + modl.m[ 7] * proj.m[12];
    clip[ 5] = modl.m[ 4] * proj.m[ 1] + modl.m[ 5] * proj.m[ 5] + modl.m[ 6] * proj.m[ 9] + modl.m[ 7] * proj.m[13];
    clip[ 6] = modl.m[ 4] * proj.m[ 2] + modl.m[ 5] * proj.m[ 6] + modl.m[ 6] * proj.m[10] + modl.m[ 7] * proj.m[14];
    clip[ 7] = modl.m[ 4] * proj.m[ 3] + modl.m[ 5] * proj.m[ 7] + modl.m[ 6] * proj.m[11] + modl.m[ 7] * proj.m[15];
    
    clip[ 8] = modl.m[ 8] * proj.m[ 0] + modl.m[ 9] * proj.m[ 4] + modl.m[10] * proj.m[ 8] + modl.m[11] * proj.m[12];
    clip[ 9] = modl.m[ 8] * proj.m[ 1] + modl.m[ 9] * proj.m[ 5] + modl.m[10] * proj.m[ 9] + modl.m[11] * proj.m[13];
    clip[10] = modl.m[ 8] * proj.m[ 2] + modl.m[ 9] * proj.m[ 6] + modl.m[10] * proj.m[10] + modl.m[11] * proj.m[14];
    clip[11] = modl.m[ 8] * proj.m[ 3] + modl.m[ 9] * proj.m[ 7] + modl.m[10] * proj.m[11] + modl.m[11] * proj.m[15];
    
    clip[12] = modl.m[12] * proj.m[ 0] + modl.m[13] * proj.m[ 4] + modl.m[14] * proj.m[ 8] + modl.m[15] * proj.m[12];
    clip[13] = modl.m[12] * proj.m[ 1] + modl.m[13] * proj.m[ 5] + modl.m[14] * proj.m[ 9] + modl.m[15] * proj.m[13];
    clip[14] = modl.m[12] * proj.m[ 2] + modl.m[13] * proj.m[ 6] + modl.m[14] * proj.m[10] + modl.m[15] * proj.m[14];
    clip[15] = modl.m[12] * proj.m[ 3] + modl.m[13] * proj.m[ 7] + modl.m[14] * proj.m[11] + modl.m[15] * proj.m[15];
    
    /* Extract the numbers for the RIGHT plane */
    frustum[0][0] = clip[ 3] - clip[ 0];
    frustum[0][1] = clip[ 7] - clip[ 4];
    frustum[0][2] = clip[11] - clip[ 8];
    frustum[0][3] = clip[15] - clip[12];
    
    /* Normalize the result */
    t = sqrt( frustum[0][0] * frustum[0][0] + frustum[0][1] * frustum[0][1] + frustum[0][2] * frustum[0][2] );
    frustum[0][0] /= t;
    frustum[0][1] /= t;
    frustum[0][2] /= t;
    frustum[0][3] /= t;
    
    /* Extract the numbers for the LEFT plane */
    frustum[1][0] = clip[ 3] + clip[ 0];
    frustum[1][1] = clip[ 7] + clip[ 4];
    frustum[1][2] = clip[11] + clip[ 8];
    frustum[1][3] = clip[15] + clip[12];
    
    /* Normalize the result */
    t = sqrt( frustum[1][0] * frustum[1][0] + frustum[1][1] * frustum[1][1] + frustum[1][2] * frustum[1][2] );
    frustum[1][0] /= t;
    frustum[1][1] /= t;
    frustum[1][2] /= t;
    frustum[1][3] /= t;
    
    /* Extract the BOTTOM plane */
    frustum[2][0] = clip[ 3] + clip[ 1];
    frustum[2][1] = clip[ 7] + clip[ 5];
    frustum[2][2] = clip[11] + clip[ 9];
    frustum[2][3] = clip[15] + clip[13];
    
    /* Normalize the result */
    t = sqrt( frustum[2][0] * frustum[2][0] + frustum[2][1] * frustum[2][1] + frustum[2][2] * frustum[2][2] );
    frustum[2][0] /= t;
    frustum[2][1] /= t;
    frustum[2][2] /= t;
    frustum[2][3] /= t;
    
    /* Extract the TOP plane */
    frustum[3][0] = clip[ 3] - clip[ 1];
    frustum[3][1] = clip[ 7] - clip[ 5];
    frustum[3][2] = clip[11] - clip[ 9];
    frustum[3][3] = clip[15] - clip[13];
    
    /* Normalize the result */
    t = sqrt( frustum[3][0] * frustum[3][0] + frustum[3][1] * frustum[3][1] + frustum[3][2] * frustum[3][2] );
    frustum[3][0] /= t;
    frustum[3][1] /= t;
    frustum[3][2] /= t;
    frustum[3][3] /= t;
    
    /* Extract the FAR plane */
    frustum[4][0] = clip[ 3] - clip[ 2];
    frustum[4][1] = clip[ 7] - clip[ 6];
    frustum[4][2] = clip[11] - clip[10];
    frustum[4][3] = clip[15] - clip[14];
    
    /* Normalize the result */
    t = sqrt( frustum[4][0] * frustum[4][0] + frustum[4][1] * frustum[4][1] + frustum[4][2] * frustum[4][2] );
    frustum[4][0] /= t;
    frustum[4][1] /= t;
    frustum[4][2] /= t;
    frustum[4][3] /= t;
    
    /* Extract the NEAR plane */
    frustum[5][0] = clip[ 3] + clip[ 2];
    frustum[5][1] = clip[ 7] + clip[ 6];
    frustum[5][2] = clip[11] + clip[10];
    frustum[5][3] = clip[15] + clip[14];
    
    /* Normalize the result */
    t = sqrt( frustum[5][0] * frustum[5][0] + frustum[5][1] * frustum[5][1] + frustum[5][2] * frustum[5][2] );
    frustum[5][0] /= t;
    frustum[5][1] /= t;
    frustum[5][2] /= t;
    frustum[5][3] /= t;
}

bool Utils::pointInFrustum(Vector3d pos)
{
    for (int p = 0; p < 6; p++) {
        if (frustum[p][0] * pos[0] + frustum[p][1] * pos[1] + frustum[p][2] * pos[2] + frustum[p][3] <= 0) {
            return false;
        }
    }
    
    return true;
}

bool Utils::sphereInFrustum(Vector3d pos, float radius)
{
    for (int p = 0; p < 6; p++) {
        if (frustum[p][0] * pos[0] + frustum[p][1] * pos[1] + frustum[p][2] * pos[2] + frustum[p][3] <= -radius) {
            return false;
        }
    }
    
    return true;
}

