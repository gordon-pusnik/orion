//
//  Kono.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-23.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Kono__
#define __Orion__Kono__

#include "Station.h"

class Kono : public Station
{
public:
    Kono();
    ~Kono();

    void init(Vector3f pos, Vector3f lightPos, Vector4f lightColor);
    void update(float dt);
    void render();
};

#endif /* defined(__Orion__Kono__) */
