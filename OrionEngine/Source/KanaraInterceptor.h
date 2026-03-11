//
//  KanaraInterceptor.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-23.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__KanaraInterceptor__
#define __Orion__KanaraInterceptor__

#include "Ship.h"

class Engine;

class KanaraInterceptor : public Ship
{
public:
    KanaraInterceptor();
    ~KanaraInterceptor();
    
    void init(Vector3f pos, Vector3f lightPos, Vector4f lightColor, bool ourShip);
    void update(float dt);
    void render();
    
private:
    Engine *m_pEngine1;
    Engine *m_pEngine2;
    Engine *m_pEngine3;
    Engine *m_pEngine4;    
};

#endif /* defined(__Orion__KanaraInterceptor__) */
