//
//  Kono.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-23.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "Kono.h"
#include "Constants.h"
#include "Utils.h"

Kono::Kono() : Station()
{
    m_type = KONO;
}

Kono::~Kono()
{
    
}

void Kono::init(Vector3f pos, Vector3f lightPos, Vector4f lightColor)
{
    Station::init(pos, "Models/konostation.obj", lightPos, lightColor, Utils::getScaled(500.0));
}

void Kono::update(float dt)
{
    Station::update(dt);
}

void Kono::render()
{
    Station::render();
}
