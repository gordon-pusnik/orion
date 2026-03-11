//
//  SpaceScene.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-01.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__SpaceScene__
#define __Orion__SpaceScene__

class Skybox;

#include "Entity.h"
#include "Scene.h"
#include "Hud.h"

class SpaceScene : public Scene
{
public:
    SpaceScene();
    ~SpaceScene();
    
    void init(float viewWidth, float viewHeight);
    void update(float dt);
    void render();
    void touchesBegan(float x, float y);
    void touchesMoved(float x, float y);
    void touchesEnded(float x, float y);
    void keyDown(unsigned short keycode);
    void keyUp(unsigned short keycode);
    void sortEntities();
    void pinchBegan();
    void pinchChanged(float scale);
    void pinchEnded();
    void panBegan();
    void panChanged(float x, float y);
    void panEnded();
    void rotateBegan();
    void rotateChanged(float rotation);
    void rotateEnded();
    SceneType type() { return SPACE_SCENE_TYPE; };
    EntityList* getEntityList() { return &m_entities; };

private:
    Skybox *m_pSkybox;
    EntityList m_entities;
    Entity *m_pShip;
    Hud *m_pHud;
};

#endif /* defined(__Orion__SpaceScene__) */
