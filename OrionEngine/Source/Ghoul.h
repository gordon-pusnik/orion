//
//  Ghoul.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-23.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Ghoul__
#define __Orion__Ghoul__

#include "Ship.h"

class Ghoul : public Ship
{
public:
    Ghoul();
    ~Ghoul();

    void init(Vector3f pos, Vector3f lightPos, Vector4f lightColor, bool ourShip);
    void update(float dt);
    void render();
};

#endif /* defined(__Orion__Ghoul__) */
