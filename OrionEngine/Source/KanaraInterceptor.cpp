//
//  KanaraInterceptor.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-23.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "KanaraInterceptor.h"
#include "Constants.h"
#include "Utils.h"
#include "Manager.h"
#include "Engine.h"

KanaraInterceptor::KanaraInterceptor() : Ship()
{
    m_type = KANARA_INTERCEPTOR;
}

KanaraInterceptor::~KanaraInterceptor()
{
    delete m_pEngine1;
    delete m_pEngine2;
    delete m_pEngine3;
    delete m_pEngine4;
}

void KanaraInterceptor::init(Vector3f pos, Vector3f lightPos, Vector4f lightColor, bool ourShip)
{
    LOGI("Loading Kanara Interceptor\n");

    if (ourShip) {
        pos[0] = Utils::getScaled(0.0);
        pos[1] = Utils::getScaled(-330.0);
        pos[2] = Utils::getScaled(-900.0);

        m_orientation[0] = kPi / 2.0 + -0.32; // Pitch
        m_orientation[1] = kPi; // Yaw
        m_orientation[2] = kPi; // Roll
        
        Vector4f color = { 0.3, 0.3, 1.0, 1.0 };
        Vector3f pos1 = { (float) Utils::getScaled(97.0), (float) Utils::getScaled(-333.0), (float) Utils::getScaled(-890.0) };
        m_pEngine1 = new Engine;
        m_pEngine1->init(pos1, color);

        Vector3f pos2 = { (float) Utils::getScaled(-97.0), (float) Utils::getScaled(-333.0), (float) Utils::getScaled(-890.0) };
        m_pEngine2 = new Engine;
        m_pEngine2->init(pos2, color);

        Vector3f pos3 = { (float) Utils::getScaled(297.0), (float) Utils::getScaled(-433.0), (float) Utils::getScaled(-890.0) };
        m_pEngine3 = new Engine;
        m_pEngine3->init(pos3, color);

        Vector3f pos4 = { (float) Utils::getScaled(-297.0), (float) Utils::getScaled(-433.0), (float) Utils::getScaled(-890.0) };
        m_pEngine4 = new Engine;
        m_pEngine4->init(pos4, color);
    }
    else {
        m_orientation[0] = kPi / 2.0;
        m_orientation[1] = kPi;
        m_orientation[2] = 0.0;
    }
    
    Ship::init(pos, "Models/kanara_interceptor.obj", lightPos, lightColor, ourShip, Utils::getScaled(5.0));

    LOGI("Finished loading Kanara Interceptor\n");
}

void KanaraInterceptor::update(float dt)
{
    if (! m_bOurShip) {
        m_vPosition[2] += Utils::getScaled(16.0);
        
        if (Manager::timeManager()->getElapsed() > 20.0) {
    //        m_vPosition[2] += Utils::getScaled(50.0);
        }
    }
    
    float power = Manager::shipController()->getFwdRev() * 10.0;
    if (power <= 10.0) {
        power = 10.0;
    }
    
    float warpPower = Manager::shipController()->getWarpLevel() * 50.0;
    if (warpPower > 0.0) {
        power = warpPower + 300.0;
    }
    
//    bool weaponActive = Manager::shipController()->isWeaponActive();

    Ship::update(dt);
    m_pEngine1->setPower(power);
    m_pEngine1->update(dt);
    m_pEngine2->setPower(power);
    m_pEngine2->update(dt);
    m_pEngine3->setPower(power);
    m_pEngine3->update(dt);
    m_pEngine4->setPower(power);
    m_pEngine4->update(dt);
}

void KanaraInterceptor::render()
{
    Ship::render();
    m_pEngine1->render();
    m_pEngine2->render();
    m_pEngine3->render();
    m_pEngine4->render();
}
