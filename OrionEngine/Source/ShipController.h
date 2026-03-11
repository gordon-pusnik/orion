//
//  ShipController.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-12-03.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__ShipController__
#define __Orion__ShipController__

#include "FwdRevController.h"
#include "PitchYawController.h"
#include "RollController.h"
#include "Button.h"
#include "SlidePanel.h"

class ShipController
{
public:
    ShipController();
    ~ShipController();
    void init(float viewWidth, float viewHeight);
    void update(float dt);
    void render();
    void touchesBegan(float x, float y);
    void touchesMoved(float x, float y);
    void touchesEnded(float x, float y);
    void keyDown(unsigned short keycode);
    void keyUp(unsigned short keycode);
    float getFwdRev();
    float getPitch();
    float getYaw();
    float getRoll();
    float getWarpSpeed();
    int getWarpLevel() {return m_iWarpLevel;};
    void advanceTarget();
    bool isWeaponActive() { return m_bFireWeapon; };
    
private:
    FwdRevController m_fwdRevController;
    PitchYawController m_pitchYawController;
    RollController m_rollController;
    Button m_fireButton;
    SlidePanel m_slidePanel;
    
    bool m_bDown;
    bool m_bUp;
    bool m_bLeft;
    bool m_bRight;
    bool m_bRollLeft;
    bool m_bRollRight;
    bool m_bForwardThruster;
    bool m_bReverseThruster;
    int m_iWarpLevel;
    bool m_bWarping;
    int m_iSelectedTarget;
    bool m_bFireWeapon;
};

#endif /* defined(__Orion__ShipController__) */
