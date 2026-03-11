//
//  CobraInterceptor.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-23.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "CobraInterceptor.h"
#include "Constants.h"
#include "Utils.h"
#include "Manager.h"

CobraInterceptor::CobraInterceptor() : Ship()
{
    m_type = COBRA_INTERCEPTOR;
}

CobraInterceptor::~CobraInterceptor()
{
    
}

void CobraInterceptor::init(Vector3f pos, Vector3f lightPos, Vector4f lightColor, bool ourShip)
{
    LOGI("Loading Cobra Fighter\n");

    if (ourShip) {
        pos[0] = Utils::getScaled(30.0);
        pos[1] = Utils::getScaled(-290.0);
        pos[2] = Utils::getScaled(-900.0);

        m_orientation[0] = 0.0;
        m_orientation[1] = kPi;
        m_orientation[2] = 0.0;
    }
    else {
        m_orientation[0] = 0.0;
        m_orientation[1] = 0.0;
        m_orientation[2] = 0.0;
    }
    
    Ship::init(pos, "Models/cobra_interceptor.obj", lightPos, lightColor, ourShip, Utils::getScaled(10.0));

    LOGI("Finished loading Cobra Fighter\n");
}

void CobraInterceptor::update(float dt)
{
    m_vPosition[2] += Utils::getScaled(18.0);
    
    if (Manager::timeManager()->getElapsed() > 20.0) {
//        m_vPosition[2] += Utils::getScaled(50.0);
    }

    Ship::update(dt);
}

void CobraInterceptor::render()
{
    Ship::render();
}
