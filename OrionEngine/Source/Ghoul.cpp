//
//  Ghoul.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-23.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "Ghoul.h"
#include "Constants.h"
#include "Utils.h"
#include "Manager.h"

Ghoul::Ghoul() : Ship()
{
    m_type = GHOUL;
}

Ghoul::~Ghoul()
{
    
}

void Ghoul::init(Vector3f pos, Vector3f lightPos, Vector4f lightColor, bool ourShip)
{
    LOGI("Loading Ghoul\n");

    if (ourShip) {
        pos[0] = Utils::getScaled(-9.0);
        pos[1] = Utils::getScaled(-290.0);
        pos[2] = Utils::getScaled(-700.0);
        
        m_orientation[0] = 4.6;
        m_orientation[1] = 0.0;
        m_orientation[2] = kPi / 2.0;
    }
    else {
        m_orientation[0] = 4.6;
        m_orientation[1] = 0.0;
        m_orientation[2] = kPi + kPi / 2.0;
    }
    
    Ship::init(pos, "Models/ghoul.obj", lightPos, lightColor, ourShip, Utils::getScaled(10.0));

    LOGI("Finished loading Ghoul\n");
}

void Ghoul::update(float dt)
{
    if (! m_bOurShip) {
        m_vPosition[2] += Utils::getScaled(16.0);
        
        if (Manager::timeManager()->getElapsed() > 20.0) {
            //        m_vPosition[2] += Utils::getScaled(50.0);
        }
    }
    
    Ship::update(dt);
}

void Ghoul::render()
{
    Ship::render();
}
