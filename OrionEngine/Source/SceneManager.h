//
//  SceneManager.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-12-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__SceneManager__
#define __Orion__SceneManager__

#include "Scene.h"
#include "GLFrameBuffer.h"

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#ifdef OPENGL_ES
#import "ViewController.h"
#endif
#endif

enum SceneState
{
    SPACE_INIT_STATE = 0,
    SPACE_STATE,
    MAP_INIT_STATE,
    MAP_STATE
};

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void init(float viewWidth, float viewHeight);
    void update(float dt);
    void render();
    void touchesBegan(float x, float y);
    void touchesMoved(float x, float y);
    void touchesEnded(float x, float y);
    void keyDown(unsigned short keycode);
    void keyUp(unsigned short keycode);
    void pinchBegan();
    void pinchChanged(float scale);
    void pinchEnded();
    void panBegan();
    void panChanged(float x, float y);
    void panEnded();
    void rotateBegan();
    void rotateChanged(float rotation);
    void rotateEnded();
    
    void setState(SceneState state) { m_state = state; };
    SceneState getState() { return m_state; };
    
    void enableMultiTouch(bool b);
    bool multiTouchEnabled() { return m_bMultiTouch; };
    
    Scene* getActiveScene() { return m_pScene; };
    
#ifdef __APPLE__
#ifdef OPENGL_ES
    ViewController *m_viewController;
    void setViewController(ViewController* viewController) { m_viewController = viewController; };
#endif
#endif
    
private:
    void initSpaceScene();
    void initMapScene();
    
    float m_fViewWidth;
    float m_fViewHeight;
    Scene *m_pScene;
    SceneList m_scenes;
    SceneState m_state;
    bool m_bMultiTouch;
    GLFrameBuffer fb;
};


#endif /* defined(__Orion__SceneManager__) */
