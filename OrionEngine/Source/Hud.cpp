//
//  Hud.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-07-04.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "Hud.h"
#include "Reticle.h"
#include "Utils.h"

Hud::Hud()
{
    m_pEntities = 0;
}

Hud::~Hud()
{
    
}

void Hud::init(float viewWidth, float viewHeight)
{
    m_reticle.init();
}

void Hud::update(float dt)
{
    
}

void Hud::render()
{
    if (m_pEntities) {
        Utils::extractFrustum();
        
        for (EntityListItor itor = m_pEntities->begin(); itor != m_pEntities->end(); ++itor) {
            Entity* e = (*itor);
            Vector3d vec;
            e->getRelativePosition(vec);
            
            if (Utils::sphereInFrustum(vec, e->getBoundingRadius())) {
                m_reticle.render(e->getPointModelViewMatrix(), e->isSelected());
            }
        }
    }
}
