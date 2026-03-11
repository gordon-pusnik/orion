//
//  SceneManager.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-12-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "SceneManager.h"
#include "SpaceScene.h"
#include "MapScene.h"
#include "Manager.h"

SceneManager::SceneManager()
{
    m_pScene = 0;
}

SceneManager::~SceneManager()
{
    for (SceneListItor itor = m_scenes.begin(); itor != m_scenes.end(); ++itor) {
        Scene* s = (*itor);
        delete s;
    }
    
    delete Manager::shipController();
}

void SceneManager::init(float viewWidth, float viewHeight)
{
    m_fViewWidth = viewWidth;
    m_fViewHeight = viewHeight;
    
    Manager::m_sShipController = new ShipController;
    Manager::shipController()->init(viewWidth, viewHeight);
    
    setState(SPACE_INIT_STATE);
    
    fb.init(viewWidth, viewHeight);
}

void SceneManager::update(float dt)
{
    if (m_state == SPACE_INIT_STATE) {
        initSpaceScene();        
    }
    else if (m_state == MAP_INIT_STATE) {
        initMapScene();
    }
    
    if (m_pScene) {
        m_pScene->update(dt);
    }
    
    fb.bind();
    if (m_pScene) {
        m_pScene->render();
    }
    fb.unbind();
}

void SceneManager::render()
{
//    if (m_pScene) {
//        m_pScene->render();
//    }
    
    fb.render();
}

void SceneManager::touchesBegan(float x, float y)
{
    if (m_pScene) {
        m_pScene->touchesBegan(x, y);
    }    
}

void SceneManager::touchesMoved(float x, float y)
{
    if (m_pScene) {
        m_pScene->touchesMoved(x, y);
    }
}

void SceneManager::touchesEnded(float x, float y)
{
    if (m_pScene) {
        m_pScene->touchesEnded(x, y);
    }    
}

void SceneManager::keyDown(unsigned short keycode)
{
    if (m_pScene) {
        m_pScene->keyDown(keycode);
    }    
}

void SceneManager::keyUp(unsigned short keycode)
{
    if (m_pScene) {
        m_pScene->keyUp(keycode);
    }    
}

void SceneManager::pinchBegan()
{
    if (m_pScene) {
        m_pScene->pinchBegan();
    }
}

void SceneManager::pinchChanged(float scale)
{
    if (m_pScene) {
        m_pScene->pinchChanged(scale);
    }
}

void SceneManager::pinchEnded()
{
    if (m_pScene) {
        m_pScene->pinchEnded();
    }
}

void SceneManager::panBegan()
{
    if (m_pScene) {
        m_pScene->panBegan();
    }
}

void SceneManager::panChanged(float x, float y)
{
    if (m_pScene) {
        m_pScene->panChanged(x, y);
    }
}

void SceneManager::panEnded()
{
    if (m_pScene) {
        m_pScene->panEnded();
    }
}

void SceneManager::rotateBegan()
{
    if (m_pScene) {
        m_pScene->rotateBegan();
    }
}

void SceneManager::rotateChanged(float rotation)
{
    if (m_pScene) {
        m_pScene->rotateChanged(rotation);
    }
}

void SceneManager::rotateEnded()
{
    if (m_pScene) {
        m_pScene->rotateEnded();
    }
}

void SceneManager::initSpaceScene()
{
    enableMultiTouch(true);
    
    if (m_pScene) {
        // Check if the current scene is in the list, otherwise add it.
        bool found = false;
        for (SceneListItor itor = m_scenes.begin(); itor != m_scenes.end(); ++itor) {
            Scene* s = (*itor);
            if (s->type() == m_pScene->type()) {
                found = true;
                break;
            }
        }
        
        if (! found) {
            // Not found, add it
            m_scenes.push_back(m_pScene);
        }
    }
    
    // Now check the list for the scene we want, if we have it set m_pScene to it, otherwise create it.
    bool found = false;
    for (SceneListItor itor = m_scenes.begin(); itor != m_scenes.end(); ++itor) {
        Scene* s = (*itor);
        if (s->type() == SPACE_SCENE_TYPE) {
            m_pScene = s;
            found = true;
            break;
        }
    }
    
    if (! found) {
        // Not found, create a new one.
        m_pScene = new SpaceScene;
        m_pScene->init(m_fViewWidth, m_fViewHeight);
    }
    
    // Restore the camera
    Manager::camera()->restoreOrientation();

    m_state = SPACE_STATE;
}

void SceneManager::initMapScene()
{
    enableMultiTouch(false);
    
    if (m_pScene) {
        // Check if the current scene is in the list, otherwise add it.
        bool found = false;
        for (SceneListItor itor = m_scenes.begin(); itor != m_scenes.end(); ++itor) {
            Scene* s = (*itor);
            if (s->type() == m_pScene->type()) {
                found = true;
                break;
            }
        }
        
        if (! found) {
            // Not found, add it
            m_scenes.push_back(m_pScene);
        }
    }

    // Now check the list for the scene we want, if we have it set m_pScene to it, otherwise create it.
    bool found = false;
    for (SceneListItor itor = m_scenes.begin(); itor != m_scenes.end(); ++itor) {
        Scene* s = (*itor);
        if (s->type() == MAP_SCENE_TYPE) {
            m_pScene = s;
            found = true;
            break;
        }
    }
    
    if (! found) {
        // Not found, create a new one.
        m_pScene = new MapScene;
        m_pScene->init(m_fViewWidth, m_fViewHeight);
    }
    
    // Reset the camera
    Manager::camera()->resetOrientation();
    
    m_state = MAP_STATE;
}

void SceneManager::enableMultiTouch(bool b)
{
    m_bMultiTouch = b;
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    if (b) {
        [m_viewController enableMultiTouch:YES];
    }
    else {
        [m_viewController enableMultiTouch:NO];
    }
#endif
#endif
}
