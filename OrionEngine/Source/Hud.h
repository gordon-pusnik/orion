//
//  Hud.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-07-04.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Hud__
#define __Orion__Hud__

#include "Reticle.h"
#include "Entity.h"

class Hud
{
public:
    Hud();
    ~Hud();
    
    void init(float viewWidth, float viewHeight);
    void update(float dt);
    void render();
    void setEntities(EntityList *entities) {m_pEntities = entities; };
    
protected:
    EntityList *m_pEntities;
    Reticle m_reticle;
};

#endif /* defined(__Orion__Hud__) */
