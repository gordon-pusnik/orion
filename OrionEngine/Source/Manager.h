//
//  Manager.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-12-30.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Manager__
#define __Orion__Manager__

#include "Renderer.h"
#include "AssetManager.h"
#include "GeometryManager.h"
#include "SceneManager.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "ShipController.h"
#include "TimeManager.h"
#include "TextureManager.h"

class Manager
{
public:
    static Renderer *m_sRenderer;
    static SceneManager *m_sSceneManager;
    static AssetManager *m_sAssetManager;
    static GeometryManager *m_sGeometryManager;
    static ShaderManager *m_sShaderManager;
    static Camera *m_sCamera;
    static Camera *m_sFrontCamera;
    static ShipController *m_sShipController;
    static TimeManager *m_sTimeManager;
    static TextureManager *m_sTextureManager;

    static Renderer* renderer() { return m_sRenderer; };
    static SceneManager* sceneManager() { return m_sSceneManager; };
    static AssetManager* assetManager() { return m_sAssetManager; };
    static GeometryManager* geometryManager() { return m_sGeometryManager; };
    static ShaderManager* shaderManager() { return m_sShaderManager; };
    static Camera* camera() { return m_sCamera; };
    static Camera* frontCamera() { return m_sFrontCamera; };
    static ShipController *shipController() { return m_sShipController; };
    static TimeManager *timeManager() { return m_sTimeManager; };
    static TextureManager *textureManager() { return m_sTextureManager; };
};

#endif /* defined(__Orion__Manager__) */
