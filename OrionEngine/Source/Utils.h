//
//  Utils.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-10-30.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Utils__
#define __Orion__Utils__

#ifdef __APPLE__
#elif WIN32
#include "stdafx.h"
#else
#include <android/log.h>
#endif

#define  LOG_TAG    "Orion"

#ifdef __APPLE__
#define  LOGI(...)  printf(__VA_ARGS__)
#define  LOGE(...)  printf(__VA_ARGS__)
#elif WIN32
#define  LOGI(...)  printf(__VA_ARGS__)
#define  LOGE(...)  printf(__VA_ARGS__)
#else
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

#include "Vector3.h"

class Entity;
class Camera;

namespace Utils
{
    double getScaled(double f);
    double getScaleRestored(double f);
    bool compareEntityByDistance(Entity* e1, Entity* e2);
    void extractFrustum();
    bool pointInFrustum(Vector3d pos);
    bool sphereInFrustum(Vector3d pos, float radius);
};

#endif /* defined(__Orion__Utils__) */
