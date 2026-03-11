//
//  Entity.h
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-01.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#ifndef __Orion__Entity__
#define __Orion__Entity__

#include "Vector3.h"
#include "Matrix4.h"

enum EntityType {
    // PLANETS and STARS
    BROWN_DWARF = 0, //  L+T Class Brown Dwarfs
    WHITE_DWARF = 1,
    STAR_M = 2, //red
    STAR_K = 3, //orange
    STAR_G = 4, //yellow
    STAR_F = 5, //white
    STAR_A = 6, //blue/white
    STAR_B = 7, //blue
    STAR_O = 8,  //blue/purple/white
    STAR_M_GIANT = 9,
    STAR_K_GIANT = 10,
    STAR_G_GIANT = 11,
    STAR_F_GIANT = 12,
    STAR_A_GIANT = 13,
    STAR_B_GIANT = 14,
    STAR_O_GIANT = 15,
    STAR_M_SUPER_GIANT = 16,
    STAR_K_SUPER_GIANT = 17,
    STAR_G_SUPER_GIANT = 18,
    STAR_F_SUPER_GIANT = 19,
    STAR_A_SUPER_GIANT = 20,
    STAR_B_SUPER_GIANT = 21,
    STAR_O_SUPER_GIANT = 22,
    STAR_M_HYPER_GIANT = 23,
    STAR_K_HYPER_GIANT = 24,
    STAR_G_HYPER_GIANT = 25,
    STAR_F_HYPER_GIANT = 26,
    STAR_A_HYPER_GIANT = 27,
    STAR_B_HYPER_GIANT = 28,
    STAR_O_HYPER_GIANT = 29, // these various stars do exist = they are transitional states and are rare
    STAR_M_WF = 30,  //Wolf-Rayet star
    STAR_B_WF = 31,  // while you do not specifically get class M,B or O WF stars,
    STAR_O_WF = 32, //  you do get red = blue and purple from the colour of the gasses = so spectral class is an easy way to define them.
    STAR_S_BH = 33, //stellar blackhole
    STAR_IM_BH = 34, //Intermediate-mass blackhole
    STAR_SM_BH = 35, //Supermassive blackhole
    WORMHOLE = 36,
    
    PLANET_GAS_GIANT = 37,
    PLANET_GAS_GIANT_RINGED = 38,
    PLANET_ROCK = 39,
    PLANET_ROCK_RINGED = 40,
    PLANET_ASTEROID = 41,
    PLANET_LAVA = 42,
    PLANET_LAVA_RINGED = 43,
    PLANET_ICE = 44,
    PLANET_ICE_RINGED = 45,
    PLANET_STORM = 46,
    PLANET_STORM_RINGED = 47,
    PLANET_TERRESTRIAL = 48,
    PLANET_TERRESTRIAL_RINGED = 49,
    PLANET_TERRESTRIAL_OCEANIC = 50,
    PLANET_TERRESTRIAL_OCEANIC_RINGED = 51,
    
    MOON_TERRESTRIAL = 52,
    MOON_TERRESTRIAL_OCEANIC = 53,
    MOON_ROCK = 54,
    MOON_ATMOSPHERIC = 55,
    MOON_ICE = 56,
    MOON_LAVA = 57,
    MOON_STORM = 58,
    
    // STATIONS
    KONO = 100,
    
    // SHIPS
    GHOUL = 200,
    KANARA_INTERCEPTOR = 201,
    ERIS = 202,
    COBRA_FIGHTER = 203,
    COBRA_INTERCEPTOR = 204,
    ANNAV = 205,
    
    // MISC
    ENGINE = 300
};

class Entity
{
public:
    Entity() {
        m_bRenderFar = false;
        m_bSelected = false;
    };
    virtual ~Entity(){};
    
    virtual void init(Vector3f position){};
    virtual void update(float dt) = 0;
    virtual void render() = 0;
    virtual void touchesBegan(float x, float y){};
    virtual void touchesMoved(float x, float y){};
    virtual void touchesEnded(float x, float y){};
    virtual void keyDown(unsigned short keycode){};
    virtual void keyUp(unsigned short keycode){};
    virtual float getBoundingRadius() { return m_fBoundingRadius; };
    virtual void getPosition(Vector3d pos) {
        pos[0] = m_vPosition[0];
        pos[1] = m_vPosition[1];
        pos[2] = m_vPosition[2];
    };
    virtual void getRelativePosition(Vector3d pos) {
        pos[0] = m_vRelPosition[0];
        pos[1] = m_vRelPosition[1];
        pos[2] = m_vRelPosition[2];
    };
    virtual bool isFar() { return m_bRenderFar; };
    virtual void setSelected(bool b) { m_bSelected = b; };
    virtual bool isSelected() { return m_bSelected; };
    virtual EntityType type() { return m_type; };
    virtual Matrix4* getPointModelViewMatrix() { return &m_pointModelViewMatrix; };
    
protected:
    EntityType  m_type;
    float m_fBoundingRadius;
    Vector3d m_vPosition;
    Vector3f m_vRelPosition;
    bool m_bRenderFar;
    bool m_bSelected;
    Matrix4 m_pointModelViewMatrix;
};

#include <list>
typedef std::list<Entity*> EntityList;
typedef EntityList::iterator EntityListItor;

#endif /* defined(__Orion__Entity__) */
