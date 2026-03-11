//
//  ShipController.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-12-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "ShipController.h"
#include "Utils.h"
#include "Constants.h"
#include <cmath>
#include "Manager.h"
#include "Scene.h"
#include "SpaceScene.h"

ShipController::ShipController()
{
    m_bDown = false;
    m_bUp = false;
    m_bLeft = false;
    m_bRight = false;
    m_bRollLeft = false;
    m_bRollRight = false;
    m_bForwardThruster = false;
    m_bReverseThruster = false;
    m_iWarpLevel = 0;
    m_iSelectedTarget = -1;
    m_bFireWeapon = false;
    m_bWarping = false;
}

ShipController::~ShipController()
{
    
}

void ShipController::init(float viewWidth, float viewHeight)
{
    Vector3f pos;
    float size = viewHeight / 8;
    
    pos[0] = viewWidth - (size / 1.2);
    pos[1] = viewHeight / 3.5f;
    pos[2] = -0.1f;
    m_fireButton.init(pos, size);
    
    pos[0] = viewWidth - (size / 1.2);
    pos[1] = viewHeight / 2.4f;
    pos[2] = -0.1f;
    m_rollController.init(pos, size);
    
    pos[0] = size / 1.2f;
    pos[1] = viewHeight / 2.4f;
    pos[2] = -0.1f;
    m_pitchYawController.init(pos, size);
    
    pos[0] = size / 1.2;
    pos[1] = viewHeight / 3.5f;
    pos[2] = -0.1f;
    m_fwdRevController.init(pos, size);

    m_slidePanel.init(viewWidth, viewHeight);
}

void ShipController::update(float dt)
{
    if (Manager::sceneManager()->getState() == SPACE_STATE) {
        if (m_bDown)
            m_pitchYawController.setY(15.0f);
        if (m_bUp)
            m_pitchYawController.setY(-15.0f);
        if (m_bLeft)
            m_pitchYawController.setX(-15.0f);
        if (m_bRight)
            m_pitchYawController.setX(15.0f);
        if (m_bRollRight)
            m_rollController.setX(15.0f);
        if (m_bRollLeft)
            m_rollController.setX(-15.0f);
        if (m_bForwardThruster)
            m_fwdRevController.setY(15.0f);
        if (m_bReverseThruster)
            m_fwdRevController.setY(-15.0f);
        
        m_fireButton.setPressed(m_bFireWeapon);

        m_fwdRevController.update(dt);
        m_pitchYawController.update(dt);
        m_rollController.update(dt);
        m_fireButton.update(dt);
        
        Camera *cam = Manager::camera();

        // Warp
        if (m_iWarpLevel) {
            cam->setSpeed(Utils::getScaled(getWarpSpeed()));
        }
        else if (m_bWarping) {
            m_bWarping = false;
            cam->setSpeed(0.0);
        }

        cam->addSpeed(Utils::getScaled(getFwdRev() * kThrust));
        
        cam->move((getYaw() * kJoystickSensitivity), (getPitch() * kJoystickSensitivity), 0.0f);
        cam->move(0.0f, 0.0f, (getRoll() * kJoystickSensitivity));
    }
    
    m_slidePanel.update(dt);
}

void ShipController::render()
{
    Manager::renderer()->enableDepthTest(false);

    if (Manager::sceneManager()->getState() == SPACE_STATE) {
        m_fwdRevController.render();
        m_pitchYawController.render();
        m_rollController.render();
        m_fireButton.render();
    }
    
    m_slidePanel.render();
    
    Manager::renderer()->enableDepthTest(true);
}

void ShipController::touchesBegan(float x, float y)
{
    if (Manager::sceneManager()->getState() == SPACE_STATE) {
        m_fwdRevController.touchesBegan(x, y);
        m_pitchYawController.touchesBegan(x, y);
        m_rollController.touchesBegan(x, y);
        m_fireButton.touchesBegan(x, y);
    }

    m_slidePanel.touchesBegan(x, y);
}

void ShipController::touchesMoved(float x, float y)
{
    if (Manager::sceneManager()->getState() == SPACE_STATE) {
        m_fwdRevController.touchesMoved(x, y);
        m_pitchYawController.touchesMoved(x, y);
        m_rollController.touchesMoved(x, y);
        m_fireButton.touchesMoved(x, y);
    }
    
    m_slidePanel.touchesMoved(x, y);
}

void ShipController::touchesEnded(float x, float y)
{
    if (Manager::sceneManager()->getState() == SPACE_STATE) {
        m_fwdRevController.touchesEnded(x, y);
        m_pitchYawController.touchesEnded(x, y);
        m_rollController.touchesEnded(x, y);
        m_fireButton.touchesEnded(x, y);
    }
    
    m_slidePanel.touchesEnded(x, y);
}

float ShipController::getWarpSpeed()
{
    float ret = 0.0;
    
    if (m_iWarpLevel > 0) {
        ret = kSpeedOfLight * powf(m_iWarpLevel, kWarpFactor);
    }
    
    return ret;
}

float ShipController::getFwdRev()
{
    return m_fwdRevController.getY();
}

float ShipController::getPitch()
{
    return m_pitchYawController.getY();
}

float ShipController::getYaw()
{
    return m_pitchYawController.getX();
}

float ShipController::getRoll()
{
    return -m_rollController.getX();
}

#ifdef WIN32
void ShipController::keyDown(unsigned short keycode)
{
//    LOGI("keycode: %d\n", keycode);
    
    switch (keycode) {
        case 87: // w - down
            m_bDown = true;
            break;
        case 83: // s - up
            m_bUp = true;
            break;
        case 65: // a - left
            m_bLeft = true;
            break;
        case 68: // d - right
            m_bRight = true;
            break;
        case 221: // ]
            m_slidePanel.pressMapButton();
            break;
        case 219: // [
            m_slidePanel.togglePanel();
            break;
        case 73: // i - thruster +
            m_bForwardThruster = true;
            break;
        case 76: // l - roll right
            m_bRollRight = true;
            break;
        case 74: // j - roll left
            m_bRollLeft = true;
            break;
        case 75: // k - thruster -
            m_bReverseThruster = true;
            break;
        case 84: // t - advance target
            advanceTarget();
            break;
        case 32: // space bar
			m_bFireWeapon = true;
            break;
        case 187: // +
            m_iWarpLevel++;
            m_bWarping = true;
            if (m_iWarpLevel > 9) {
                m_iWarpLevel = 9;
            }
            break;
        case 189: // -
            m_iWarpLevel--;
            if (m_iWarpLevel < 0) {
                m_iWarpLevel = 0;
            }
            break;
    };
}

void ShipController::keyUp(unsigned short keycode)
{
    switch (keycode) {
        case 87: // w - down
			m_pitchYawController.setY(0.0f);
            m_bDown = false;
            break;
        case 83: // s - up
            m_pitchYawController.setY(0.0f);
            m_bUp = false;
            break;
        case 65: // a - left
            m_pitchYawController.setX(0.0f);
            m_bLeft = false;
            break;
        case 68: // d - right
            m_pitchYawController.setX(0.0f);
            m_bRight = false;
            break;
        case 221: // ]
            m_slidePanel.releaseMapButton();
            break;
        case 73: // i - thrust +
            m_fwdRevController.setY(0.0f);
            m_bForwardThruster = false;
            break;
        case 76: // l - roll right
            m_rollController.setX(0.0f);
            m_bRollRight = false;
            break;
        case 74: // j - roll left
            m_rollController.setX(0.0f);
            m_bRollLeft = false;
            break;
        case 75: // k - thrust -
            m_fwdRevController.setY(0.0f);
            m_bReverseThruster = false;
            break;
        case 32:
            m_bFireWeapon = false;
            break;
    };
}
#else
void ShipController::keyDown(unsigned short keycode)
{
//    LOGI("keycode: %d\n", keycode);
    
    switch (keycode) {
        case 13: // w - down
            m_bDown = true;
            break;
        case 1: // s - up
            m_bUp = true;
            break;
        case 0: // a - left
            m_bLeft = true;
            break;
        case 2: // d - right
            m_bRight = true;
            break;
        case 30: // ]
            m_slidePanel.pressMapButton();
            break;
        case 33: // [
            m_slidePanel.togglePanel();
            break;
        case 34: // i - thruster +
            m_bForwardThruster = true;
            break;
        case 37: // l - roll right
            m_bRollRight = true;
            break;
        case 38: // j - roll left
            m_bRollLeft = true;
            break;
        case 40: // k - thruster -
            m_bReverseThruster = true;
            break;
        case 17: // t - advance target
            advanceTarget();
            break;
        case 49: // space bar
			m_bFireWeapon = true;
            break;
        case 24: // +
            m_iWarpLevel++;
            m_bWarping = true;
            if (m_iWarpLevel > 9) {
                m_iWarpLevel = 9;
            }
            break;
        case 27: // -
            m_iWarpLevel--;
            if (m_iWarpLevel < 0) {
                m_iWarpLevel = 0;
            }
            break;
    };
}

void ShipController::keyUp(unsigned short keycode)
{
    switch (keycode) {
        case 13: // w - down
            m_pitchYawController.setY(0.0f);
            m_bDown = false;
            break;
        case 1: // s - up
            m_pitchYawController.setY(0.0f);
            m_bUp = false;
            break;
        case 0: // a - left
            m_pitchYawController.setX(0.0f);
            m_bLeft = false;
            break;
        case 2: // d - right
            m_pitchYawController.setX(0.0f);
            m_bRight = false;
            break;
        case 30: // ]
            m_slidePanel.releaseMapButton();
            break;
        case 34: // i - thrust +
            m_fwdRevController.setY(0.0f);
            m_bForwardThruster = false;
            break;
        case 37: // l - roll right
            m_rollController.setX(0.0f);
            m_bRollRight = false;
            break;
        case 38: // j - roll left
            m_rollController.setX(0.0f);
            m_bRollLeft = false;
            break;
        case 40: // k - thrust -
            m_fwdRevController.setY(0.0f);
            m_bReverseThruster = false;
            break;
        case 49:
            m_bFireWeapon = false;
            break;
    };
}
#endif

void ShipController::advanceTarget()
{
    Scene *scene = Manager::sceneManager()->getActiveScene();
    if (scene && scene->type() == SPACE_SCENE_TYPE) {
        SpaceScene *spaceScene = (SpaceScene*) scene;
        EntityList *entities = spaceScene->getEntityList();
        
        for (EntityListItor itor = entities->begin(); itor != entities->end(); ++itor) {
            Entity* e = (*itor);
            e->setSelected(false);
        }

        int num = (int) entities->size();
        int i = 0;
        for (EntityListItor itor = entities->begin(); itor != entities->end(); ++itor) {
            Entity* e = (*itor);
            if (/*e->type() >= 200 && e->type() < 300*/1) {
                if (m_iSelectedTarget == -1) {
                    e->setSelected(true);
                    m_iSelectedTarget = i;
                    break;
                }
                else if (i == m_iSelectedTarget + 1) {
                    e->setSelected(true);
                    m_iSelectedTarget = i;
                    break;
                }
                else if (i == num - 1) {
                    m_iSelectedTarget = -1;
                }
            }
            i++;
        }
    }
}
