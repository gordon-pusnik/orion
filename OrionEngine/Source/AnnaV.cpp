//
//  AnnaV.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-23.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "AnnaV.h"
#include "Constants.h"
#include "Utils.h"

AnnaV::AnnaV() : Ship()
{
    m_type = ANNAV;
}

AnnaV::~AnnaV()
{
    
}

void AnnaV::init(Vector3f pos, Vector3f lightPos, Vector4f lightColor, bool ourShip)
{
    LOGI("Loading Anna V\n");

    if (ourShip) {
        pos[0] = Utils::getScaled(30.0);
        pos[1] = Utils::getScaled(-290.0);
        pos[2] = Utils::getScaled(-900.0);

        m_orientation[0] = 0.0;
        m_orientation[1] = kPi;
        m_orientation[2] = 0.0;
    }
    else {
        m_orientation[0] = kPi / 2.0;
        m_orientation[1] = kPi;
        m_orientation[2] = kPi / 2.0;
    }
    
    Ship::init(pos, "Models/anna-v.obj", lightPos, lightColor, ourShip, Utils::getScaled(65.0));

    LOGI("Finished loading Anna V\n");
}

void AnnaV::update(float dt)
{
    m_vPosition[2] += Utils::getScaled(3.0);
    
    Ship::update(dt);
}

void AnnaV::render()
{
    Ship::render();
}
