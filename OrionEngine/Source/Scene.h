//
//  Scene.h
//  Orion
//
//  Created by Gordon Pusnik on 2013-01-31.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Scene__
#define __Orion__Scene__

enum SceneType
{
    SPACE_SCENE_TYPE = 0,
    MAP_SCENE_TYPE,
};

class Scene
{
public:
    Scene(){};
    virtual ~Scene() = 0;
    
    virtual void init(float viewWidth, float viewHeight) = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void touchesBegan(float x, float y) = 0;
    virtual void touchesMoved(float x, float y) = 0;
    virtual void touchesEnded(float x, float y) = 0;
    virtual void keyDown(unsigned short keycode) = 0;
    virtual void keyUp(unsigned short keycode) = 0;
    virtual void pinchBegan() = 0;
    virtual void pinchChanged(float scale) = 0;
    virtual void pinchEnded() = 0;
    virtual void panBegan() = 0;
    virtual void panChanged(float x, float y) = 0;
    virtual void panEnded() = 0;
    virtual void rotateBegan() = 0;
    virtual void rotateChanged(float rotation) = 0;
    virtual void rotateEnded() = 0;
    virtual SceneType type() = 0;
};

#include <list>
typedef std::list<Scene*> SceneList;
typedef SceneList::iterator SceneListItor;

#endif /* defined(__Orion__Scene__) */
