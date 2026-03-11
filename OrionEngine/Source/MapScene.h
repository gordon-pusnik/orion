//
//  MapScene.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-01.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__MapScene__
#define __Orion__MapScene__

#include "Scene.h"
#include "GLBatch.h"
#include "GLPointSpriteStarEffect2.h"
#include "Vector3.h"
#include "Vector2.h"

class Imposter;
class Nebula;
class GalaxyEngine;

struct Color
{
    double r;
    double g;
    double b;
};

class MapScene : public Scene
{
public:
    MapScene();
    ~MapScene();
    
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
    SceneType type() { return MAP_SCENE_TYPE; };
    Color colorFromTemperature(double temp) const;

private:
    
    Imposter *m_pMilkywayBackground;
    Nebula *m_pNebula;
    GalaxyEngine *m_pGalaxyEngine;
    
    Vector3f m_vPosition;
    GLBatch *m_pPointsBatch;
    GLPointSpriteStarEffect2 *m_pEffect;
    GLuint m_uiTextureID;
    
    Vector2f m_vStartPoint;
    float m_fRotX;
    float m_fRotY;
    float m_fRotZ;
    
    bool m_bGesturing;
    
    float m_fPinchInitial;
    float m_fScale;
    
    bool m_bScaleUp;
    bool m_bScaleDown;
    bool m_bPanLeft;
    bool m_bPanRight;
    bool m_bPanUp;
    bool m_bPanDown;
    bool m_bRotateLeft;
    bool m_bRotateRight;
    
    float m_fPanInitialX;
    float m_fPanInitialY;
    float m_fRotInitial;
    
    float m_fPanX;
    float m_fPanY;
    
    // Star color management
    int m_colNum;
    double m_t0, m_t1, m_dt;
    Color m_col[200];
};

#endif /* defined(__Orion__MapScene__) */
