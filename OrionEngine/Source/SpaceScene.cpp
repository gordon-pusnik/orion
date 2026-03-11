//
//  SpaceScene.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-01.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "SpaceScene.h"
#include "Manager.h"
#include "Planet.h"
#include "Skybox.h"
#include "Star.h"
#include "Constants.h"
#include "Utils.h"
#include "MultitexPlanet.h"
#include <string>
#include "GLUtils.h"
#include "Starfield.h"
#include "Ship.h"
#include "Ghoul.h"
#include "CobraFighter.h"
#include "CobraInterceptor.h"
#include "KanaraInterceptor.h"
#include "Eris.h"
#include "AnnaV.h"
#include "Kono.h"

SpaceScene::SpaceScene()
{
    m_pSkybox = 0;
    m_pShip = 0;
    m_pHud = 0;
}

SpaceScene::~SpaceScene()
{
    delete m_pSkybox;
    delete m_pShip;
    delete m_pHud;
    
    for (EntityListItor itor = m_entities.begin(); itor != m_entities.end(); ++itor) {
        Entity* e = (*itor);
        delete e;
    }
}

void SpaceScene::init(float viewWidth, float viewHeight)
{
    LOGI("Creating new space scene\n");
    
    m_pSkybox = new Skybox;
    Vector3f possky = { 0.0f, 0.0f, 0.0f };
    m_pSkybox->init(possky, 100);
    
//    m_pStarfield = new Starfield;

    Star *s = new Star;

    Vector3f poss = { (float) Utils::getScaled(kPosSolX), (float) Utils::getScaled(kPosSolY), (float) Utils::getScaled(kPosSolZ) };

    Vector4f vColor = { 0.75f, 0.74f, 0.68f, 1.0f };
    s->init(STAR_G, poss, Utils::getScaled(kRadiusSol), "Images/Sun.png", vColor, 0.000005f);
    m_entities.push_back(s);

    Vector3f vLightPos = { (float) Utils::getScaled(kPosSolX), (float) Utils::getScaled(kPosSolY), (float) Utils::getScaled(kPosSolZ) };
    Vector4f vLightColor = { 1.0f, 1.0f, 1.0f, 1.0f };

    poss[0] = 0.0;
    poss[2] = Utils::getScaled(-50000000.0);
    Planet *pm = new Planet;
    pm->init(PLANET_TERRESTRIAL, poss, (float) Utils::getScaled(kRadiusEarth), "Images/Orion6.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(pm);    

    poss[2] = Utils::getScaled(0.0);
    poss[0] = Utils::getScaled(384000000.0);
    Planet *px = new Planet;
    px->init(PLANET_ROCK, poss, (float) Utils::getScaled(kRadiusMoon), "Images/Moon.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(px);
    
    poss[0] = Utils::getScaled(18000.0);
    poss[1] = Utils::getScaled(-5000.0);
    poss[2] = Utils::getScaled(-50000.0);

    Ghoul *ghoul = new Ghoul;
    ghoul->init(poss, vLightPos, vLightColor, false);
    m_entities.push_back(ghoul);

    // Position front camera in front of our ship
    Vector3f v = {0.0, 0.0, -2000.0};
    Manager::m_sFrontCamera = new Camera;
    Manager::frontCamera()->init(v, viewWidth, viewHeight);
    
    poss[0] = Utils::getScaled(20000.0);
    poss[1] = Utils::getScaled(-2000.0);
    poss[2] = Utils::getScaled(-35000.0);
    
    Eris *eris = new Eris;
    eris->init(poss, vLightPos, vLightColor, false);
    m_entities.push_back(eris);

    poss[0] = Utils::getScaled(18000.0);
    poss[1] = Utils::getScaled(-4000.0);
    poss[2] = Utils::getScaled(-40000.0);
    
    CobraFighter *cobraFighter = new CobraFighter;
    cobraFighter->init(poss, vLightPos, vLightColor, false);
    m_entities.push_back(cobraFighter);
    
    poss[0] = Utils::getScaled(18000.0);
    poss[1] = Utils::getScaled(-3000.0);
    poss[2] = Utils::getScaled(-45000.0);
    
    CobraInterceptor *cobraInterceptor = new CobraInterceptor;
    cobraInterceptor->init(poss, vLightPos, vLightColor, false);
    m_entities.push_back(cobraInterceptor);

    poss[0] = Utils::getScaled(18000.0);
    poss[1] = Utils::getScaled(-5000.0);
    poss[2] = Utils::getScaled(-50000.0);
    
    KanaraInterceptor *kanaraInterceptor = new KanaraInterceptor;
    kanaraInterceptor->init(poss, vLightPos, vLightColor, true);
    //m_entities.push_back(kanaraInterceptor);
    m_pShip = kanaraInterceptor;
    
    poss[0] = Utils::getScaled(27000.0);
    poss[1] = Utils::getScaled(-2000.0);
    poss[2] = Utils::getScaled(-25000.0);
    
    AnnaV *annav = new AnnaV;
    annav->init(poss, vLightPos, vLightColor, false);
    m_entities.push_back(annav);
    
    poss[0] = Utils::getScaled(2000.0);
    poss[1] = 0.0;
    poss[2] = Utils::getScaled(-80000.0);
    
    Kono *kono = new Kono;
    kono->init(poss, vLightPos, vLightColor);
    m_entities.push_back(kono);
    
    m_pHud = new Hud;
    m_pHud->init(viewWidth, viewHeight);
    m_pHud->setEntities(&m_entities);
    
/*
    // Mercury
    poss[2] = Utils::getScaled(kPosSolZ) + Utils::getScaled(-kDistanceMercury);
    Planet *pm = new Planet;
    pm->init(PLANET_ROCKY, poss, (float) Utils::getScaled(kRadiusMercury), "Images/Mercury.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(pm);

    // Venus
    poss[2] = Utils::getScaled(kPosSolZ) + Utils::getScaled(-kDistanceVenus);
    Planet *pv = new Planet;
    pv->init(PLANET_ROCKY, poss, (float) Utils::getScaled(kRadiusVenus), "Images/Venus.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(pv);

    // Earth
    poss[2] = Utils::getScaled(kPosSolZ) + Utils::getScaled(-kDistanceEarth);
    MultitexPlanet *pearth = new MultitexPlanet;
    pearth->init(PLANET_TERRESTRIAL, poss, (float) Utils::getScaled(kRadiusEarth), "Images/EarthDay.png", "Images/EarthNight.png", "Images/EarthClouds.png", vLightPos, vLightColor, 4.7f, 0.000005f);
    m_entities.push_back(pearth);

    // Moon
    poss[0] = Utils::getScaled(kDistanceMoon);
    poss[2] = Utils::getScaled(kPosSolZ) + Utils::getScaled(-kDistanceEarth);
    Planet *pmoon = new Planet;
    pmoon->init(PLANET_ROCKY, poss, (float) Utils::getScaled(kRadiusMoon), "Images/Moon.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(pmoon);

    // Mars
    poss[2] = Utils::getScaled(kPosSolZ) + Utils::getScaled(-kDistanceMars);
    Planet *pmars = new Planet;
    pmars->init(PLANET_ROCKY, poss, (float) Utils::getScaled(kRadiusMars), "Images/Mars.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(pmars);

    // Jupiter
    vLightPos[2] = -poss[2];
    poss[2] = Utils::getScaled(kPosSolZ) + Utils::getScaled(-kDistanceJupiter);
    Planet *pjupiter = new Planet;
    pjupiter->init(PLANET_GAS_GIANT, poss, (float) Utils::getScaled(kRadiusJupiter), "Images/Jupiter.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(pjupiter);
*/   
    // Saturn
    poss[0] = 0.0;
    poss[2] = Utils::getScaled(kPosSolZ) + Utils::getScaled(-kDistanceSaturn);
    Planet *psaturn = new Planet;
    psaturn->init(PLANET_GAS_GIANT, poss, (float) Utils::getScaled(kRadiusSaturn), "Images/Saturn.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(psaturn);
/*
    // Uranus
    poss[2] = Utils::getScaled(kPosSolZ) + Utils::getScaled(-kDistanceUranus);
    Planet *puranus = new Planet;
    puranus->init(PLANET_GAS_GIANT, poss, (float) Utils::getScaled(kRadiusUranus), "Images/Uranus.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(puranus);
*/
    // Neptune
    poss[2] = Utils::getScaled(kPosSolZ) + Utils::getScaled(kDistanceNeptune);
    Planet *pneptune = new Planet;
    pneptune->init(PLANET_GAS_GIANT, poss, (float) Utils::getScaled(kRadiusNeptune), "Images/Neptune.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(pneptune);
/*
    // Pluto
    poss[2] = Utils::getScaled(kPosSolZ) + Utils::getScaled(-kDistancePluto);
    Planet *ppluto = new Planet;
    ppluto->init(PLANET_ROCKY, poss, (float) Utils::getScaled(kRadiusPluto), "Images/Pluto.png", vLightPos, vLightColor, 0.0f, 0.000005f);
    m_entities.push_back(ppluto);
*/
/*
    int numStars = 5000;
    GLfloat vertexes[numStars * 7];
    int j = 0;
    for (int i = 0; i < numStars; i++) {
        float num = Utils::randomInRange(0, 100);
        float scale = num / 100000.0f;
        float x = scale * 4.12e16;
        num = Utils::randomInRange(0, 100);
        if (num > 50) {
            // Negative
            x = -x;
        }
        
        num = Utils::randomInRange(0, 100);
        scale = num / 100000.0f;
        float y = scale * 4.12e16;
        num = Utils::randomInRange(0, 100);
        if (num > 50) {
            // Negative
            y = -y;
        }

        num = Utils::randomInRange(0, 100);
        scale = num / 100000.0f;
        float z = scale * 4.12e16;
        num = Utils::randomInRange(0, 100);
        if (num > 50) {
            // Negative
            z = -z;
        }
        
//        LOGI("x: %f, y: %f, z: %f\n", x, y, z);
        
        num = Utils::randomInRange(70, 100);
        float r = (float) num / 100.0f;

        num = Utils::randomInRange(70, 100);
        float g = (float) num / 100.0f;
        
        num = Utils::randomInRange(70, 100);
        float b = (float) num / 100.0f;
        
//        LOGI("r: %f, g: %f, b: %f\n", r, g, b);
        
        num = Utils::randomInRange(1, 10);
        float radius = (float) num / 10.0;
        
//        LOGI("radius: %f\n", radius);
        
        Star *s = new Star;
        Vector3f pos = { (float) Utils::getScaled(x), (float) Utils::getScaled(y), (float) Utils::getScaled(z) };
        Vector4f vColor = { r, g, b, 1.0f };
        s->init(STAR_K, pos, Utils::getScaled(kRadiusSol * radius), "Images/Sun.png", vColor, 0.000005f);

        float distance = sqrt(pos[0]*pos[0] + pos[1]*pos[1] + pos[2]*pos[2]);
        if (distance < Utils::getScaled(kDistMaxVisibleStar)) {
            m_entities.push_back(s);
        }
        
        vertexes[j++] = pos[0];
        vertexes[j++] = pos[1];
        vertexes[j++] = pos[2];
        vertexes[j++] = vColor[0];
        vertexes[j++] = vColor[1];
        vertexes[j++] = vColor[2];
        vertexes[j++] = vColor[3];
    }
*/ 
//    m_pStarfield->init(vertexes, numStars);

    sortEntities();
 
//    LOGI("Num entities: %ld\n", m_entities.size());
}

void SpaceScene::update(float dt)
{
    Manager::shipController()->update(dt);
    Manager::camera()->update(dt);
    Manager::frontCamera()->update(dt);

    m_pSkybox->update(dt);

    
    for (EntityListItor itor = m_entities.begin(); itor != m_entities.end(); ++itor) {
        Entity* e = (*itor);
        e->update(dt);
    }
    
    m_pShip->update(dt);
}

void SpaceScene::render()
{
    Renderer *renderer = Manager::renderer();
    renderer->clearScreen();
    
    m_pSkybox->render();

    Utils::extractFrustum();
    
    for (EntityListItor itor = m_entities.begin(); itor != m_entities.end(); ++itor) {
        Entity* e = (*itor);
        if (! e->isFar()) {
            Vector3d vec;
            e->getRelativePosition(vec);

            if (Utils::sphereInFrustum(vec, e->getBoundingRadius())) {
                e->render();
            }
        }
    }
    
    m_pHud->render();
    
    m_pShip->render();

    Manager::shipController()->render();
}

void SpaceScene::touchesBegan(float x, float y)
{
    Manager::shipController()->touchesBegan(x, y);
}

void SpaceScene::touchesMoved(float x, float y)
{
    Manager::shipController()->touchesMoved(x, y);
}

void SpaceScene::touchesEnded(float x, float y)
{
    Manager::shipController()->touchesEnded(x, y);
}

void SpaceScene::keyDown(unsigned short keycode)
{
    Manager::shipController()->keyDown(keycode);
}

void SpaceScene::keyUp(unsigned short keycode)
{
    Manager::shipController()->keyUp(keycode);
}

void SpaceScene::sortEntities()
{
    m_entities.sort(Utils::compareEntityByDistance);    
}

void SpaceScene::pinchBegan()
{
    
}

void SpaceScene::pinchChanged(float scale)
{
    
}

void SpaceScene::pinchEnded()
{
    
}

void SpaceScene::panBegan()
{
    
}

void SpaceScene::panChanged(float x, float y)
{
    
}

void SpaceScene::panEnded()
{
    
}

void SpaceScene::rotateBegan()
{
    
}

void SpaceScene::rotateChanged(float rotation)
{
    
}

void SpaceScene::rotateEnded()
{
    
}


