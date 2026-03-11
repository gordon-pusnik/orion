//
//  Manager.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-12-30.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Manager.h"

Renderer* Manager::m_sRenderer = 0;
AssetManager* Manager::m_sAssetManager = 0;
GeometryManager* Manager::m_sGeometryManager = 0;
ShaderManager* Manager::m_sShaderManager = 0;
SceneManager* Manager::m_sSceneManager = 0;
Camera* Manager::m_sCamera = 0;
Camera* Manager::m_sFrontCamera = 0;
ShipController* Manager::m_sShipController = 0;
TimeManager* Manager::m_sTimeManager = 0;
TextureManager* Manager::m_sTextureManager = 0;
