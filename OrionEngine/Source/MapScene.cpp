//
//  MapScene.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-01.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "MapScene.h"
#include "Manager.h"
#include "Imposter.h"
#include "Utils.h"
#include "GLUtils.h"
#include "Constants.h"
#include "Nebula.h"
#include "GalaxyEngine.h"
#include "SpecRend.h"
#include "Random.h"
#include <algorithm>

#ifdef __APPLE__
#ifndef OPENGL_ES
#include <OpenGL/gl.h>
#endif
#endif

MapScene::MapScene()
{
    m_pMilkywayBackground = 0;
	m_pNebula = 0;
	m_pEffect = 0;
	m_pGalaxyEngine = 0;
	m_pPointsBatch = 0;
    m_fScale = 1.0;
    m_bGesturing = false;
    m_bScaleUp = false;
    m_bScaleDown = false;
    m_bPanLeft = false;
    m_bPanRight = false;
    m_bPanUp = false;
    m_bPanDown = false;
    m_bRotateLeft = false;
    m_bRotateRight = false;
    m_fPanX = 0.0;
    m_fPanY = 0.0;
    m_fRotX = 0.0;
    m_fRotY = 0.0;
    m_fRotZ = 0.0;
}

MapScene::~MapScene()
{
    delete m_pMilkywayBackground;
    delete m_pNebula;
    delete m_pEffect;
    delete m_pGalaxyEngine;
    delete m_pPointsBatch;
    glDeleteTextures(1, &m_uiTextureID);
}

void MapScene::init(float viewWidth, float viewHeight)
{    
    LOGI("Creating new map scene\n");
/*
    Vector3f pos = {0.0, 0.0, -40.0};
    m_pMilkywayBackground = new Imposter;
    m_pMilkywayBackground->init(pos, 40.0, "Images/Milkyway.png", BLEND_ALPHA, false, false);
    
    Vector4f color = {0.1, 0.1, 0.25, 0.4};
    m_pNebula = new Nebula;
    m_pNebula->init(pos, 300, 35.0, 35.0, 10.0, color, BLEND_ALPHA, "Images/smoke_transparent.png");
*/    
    glGenTextures(1, &m_uiTextureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

#ifndef OPENGL_ES
    glEnable(GL_POINT_SPRITE);
#endif
    
    if (GLUtils::loadPNGTexture("Images/halo.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
        LOGI("Star texture loaded\n");
    }

/*
    long lSize;
    FILE *pFile = Manager::assetManager()->filePtrFromAsset("Binary/milkyway0.png", &lSize);
    if (pFile) {
        size_t result;
                
        // allocate memory to contain the whole file.
        float *buffer = new float[lSize / sizeof(float)];
        
        // copy the file into the buffer.
        result = fread(buffer, sizeof(float), lSize / sizeof(float), pFile);
        
        // terminate.
        fclose(pFile);
        
        Manager::assetManager()->closeAsset();
 	
        int numStars = (int) lSize / (sizeof(float) * 10);
        
        LOGI("Successfully loaded milkyway points: %d\n", numStars);
        
        m_pEffect = new GLPointSpriteStarEffect2;
        m_pEffect->init();
        
        m_pPointsBatch = new GLBatch;
        m_pPointsBatch->beginInterleaved(GL_POINTS, numStars, 0);
        m_pPointsBatch->copyInterleavedVertexSizeColorTexCoordData((GLfloat*)buffer);
        m_pPointsBatch->endInterleavedVertexSizeColorTexCoord();
        
        delete[] buffer;
    }
*/
    
    m_colNum  = 200;
    m_t0 = 1000;
    m_t1 = 10000;
    m_dt = (m_t1 - m_t0) / m_colNum;

    double x, y, z;
    for (int i = 0; i < m_colNum; ++i) {
        Color &col = m_col[i];
        colourSystem *cs = &SMPTEsystem;
        bbTemp = m_t0 + m_dt*i;
        spectrum_to_xyz(bb_spectrum, &x, &y, &z);
        xyz_to_rgb(cs, x, y, z, &col.r, &col.g, &col.b);
        norm_rgb(&col.r, &col.g, &col.b);
    }

    Random::seedRand(987654321);
    m_pGalaxyEngine = new GalaxyEngine(13000,
                                       4000,
                                       0.0004,
                                       0.9,
                                       0.9,
                                       0.5,
                                       200,
                                       300,
                                       30000);
        
    int numStars = m_pGalaxyEngine->GetNumStars();
    GalaxyStar *stars = m_pGalaxyEngine->GetStars();
    
    int numDust = m_pGalaxyEngine->GetNumDust();
    GalaxyStar *dust = m_pGalaxyEngine->GetDust();
    
    int numH2 = m_pGalaxyEngine->GetNumH2();
    GalaxyStar *h2 = m_pGalaxyEngine->GetH2();
    
//    int numBulge = 10000;
    
    LOGI("numStars: %d, numDust: %d, numH2: %d\n", numStars, numDust, numH2);
    
    int numVerts = numStars + numDust + numH2; //+ numBulge;
    
    // Vertex (3), point size (1), color (4), index (1)
    float *buffer = new float[9 * numVerts];
    int j = 0;
    int index = 0;
    for (int i = 0; i < numStars; i++) {
 //       LOGI("%f, %f, %f\n", stars[i].m_pos.x, stars[i].m_pos.y, stars[i].m_pos.z);
        
        buffer[j++] = stars[i].m_pos.x;
        buffer[j++] = stars[i].m_pos.y;
        buffer[j++] = stars[i].m_pos.z;
        
        buffer[j++] = 1.0;
        
        const Color &col = colorFromTemperature(stars[i].m_temp);

        buffer[j++] = col.r * stars[i].m_mag;
        buffer[j++] = col.g * stars[i].m_mag;
        buffer[j++] = col.b * stars[i].m_mag;
        buffer[j++] = 1.0;
        
        buffer[j++] = (float)index;
        index++;
    }

    for (int i = 0; i < numDust; i++) {
        //        LOGI("%f, %f, %f\n", stars[i].m_pos.x, stars[i].m_pos.y, stars[i].m_pos.z);
        
        buffer[j++] = dust[i].m_pos.x;
        buffer[j++] = dust[i].m_pos.y;
        buffer[j++] = dust[i].m_pos.z;
        
        buffer[j++] = 10;
        
        const Color &col = colorFromTemperature(dust[i].m_temp);
        
        buffer[j++] = col.r * dust[i].m_mag;
        buffer[j++] = col.g * dust[i].m_mag;
        buffer[j++] = col.b * dust[i].m_mag;
        buffer[j++] = 0.6;
        
        buffer[j++] = -1.0;
    }

    for (int i = 0; i < numH2; i++) {
        //        LOGI("%f, %f, %f\n", stars[i].m_pos.x, stars[i].m_pos.y, stars[i].m_pos.z);
        
        int k1 = 2*i;
        int k2 = 2*i+1;
        
        const Vector3dC &p1 = h2[k1].m_pos;
        const Vector3dC &p2 = h2[k2].m_pos;
        
        double dst = sqrt( (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
        double size = ((1000-dst) / 10) - 70;
        if (size<1)
            continue;

        buffer[j++] = p1.x;
        buffer[j++] = p1.y;
        buffer[j++] = p1.z;
        
        buffer[j++] = 2 * size;
        //buffer[j++] = size;
        
        const Color &col = colorFromTemperature(h2[k1].m_temp);
        
        buffer[j++] = col.r * h2[i].m_mag * 2.0;
        buffer[j++] = col.g * h2[i].m_mag;
        buffer[j++] = col.b * h2[i].m_mag;
        buffer[j++] = 1.0;
        
        buffer[j++] = -1.0;
    }
/*
    // Bulge
    for (int i = 0; i < numBulge / 2; i++) {
        float theta = float(Utils::randomInRange(0.0, 2.0 * kPi));
        float u = float(Utils::randomInRange(-1.0, 1.0));
        float dist = float(Utils::randomInRange(0.0, 30.0));
        
        float x = Utils::getScaled(dist * 1000000000 * sqrt(1.0f - u * u) * cos(theta));
        float y = Utils::getScaled(dist * 1000000000 * u);
        float z = Utils::getScaled(dist * 1000000000 * sqrt(1.0f - u * u) * sin(theta));
        
        LOGI("x: %f, y: %f, z: %f\n", x, y, z);
        
        buffer[j++] = x;
        buffer[j++] = y;
        buffer[j++] = z;
        
        buffer[j++] = 10;
        
        const Color &col = colorFromTemperature(dust[i].m_temp);
        
        buffer[j++] = col.r * dust[i].m_mag;
        buffer[j++] = col.g * dust[i].m_mag;
        buffer[j++] = col.b * dust[i].m_mag;
        buffer[j++] = 1.0;
        
        buffer[j++] = 1.0;
        buffer[j++] = 1.0;
    }
    
    for (int i = 0; i < numBulge; i++) {
        float theta = float(Utils::randomInRange(0.0, 2.0 * kPi));
        float u = float(Utils::randomInRange(-1.0, 1.0));
        float dist = float(Utils::randomInRange(0.0, 20.0));
        
        float x = Utils::getScaled(dist * 1000000000 * sqrt(1.0f - u * u) * cos(theta));
        float y = Utils::getScaled(dist * 1000000000 * u);
        float z = Utils::getScaled(dist * 1000000000 * sqrt(1.0f - u * u) * sin(theta));
        
        LOGI("x: %f, y: %f, z: %f\n", x, y, z);
        
        buffer[j++] = x;
        buffer[j++] = y;
        buffer[j++] = z;
        
        buffer[j++] = 1;
        
        const Color &col = colorFromTemperature(stars[i].m_temp);
        
        buffer[j++] = col.r * stars[i].m_mag;
        buffer[j++] = col.g * stars[i].m_mag;
        buffer[j++] = col.b * stars[i].m_mag;
        buffer[j++] = 1.0;
        
        buffer[j++] = 1.0;
        buffer[j++] = 1.0;
    }
*/
    m_pEffect = new GLPointSpriteStarEffect2;
    m_pEffect->init();
    m_pEffect->setSelected(613.0);
    
    m_pPointsBatch = new GLBatch;
    m_pPointsBatch->beginInterleaved(GL_POINTS, numVerts, 0);
    m_pPointsBatch->copyInterleavedVertexSizeColorIndexData((GLfloat*)buffer);
    m_pPointsBatch->endInterleavedVertexSizeColorIndex();
    
    delete[] buffer;
}

void MapScene::update(float dt)
{    
    if (m_bScaleUp) {
        m_fScale -= 0.01 * dt * 5.0;
    }
    if (m_bScaleDown) {
        m_fScale += 0.01 * dt * 5.0;
    }
    if (m_bPanLeft) {
        m_fPanX -= 100.0 * dt * 5.0;;
    }
    if (m_bPanRight) {
        m_fPanX += 100.0 * dt * 5.0;;
    }
    if (m_bPanUp) {
        m_fPanY -= 100.0 * dt * 5.0;;
    }
    if (m_bPanDown) {
        m_fPanY += 100.0 * dt * 5.0;;
    }
    if (m_bRotateLeft) {
        m_fRotZ += 0.01 * dt * 5.0;;
    }
    if (m_bRotateRight) {
        m_fRotZ -= 0.01 * dt * 5.0;;
    }    
    
    Manager::camera()->update(dt);
    Manager::shipController()->update(dt);
/*
    m_pMilkywayBackground->rotate(m_fRotX, m_fRotY);
    m_pMilkywayBackground->scale(m_fScale);
    m_pMilkywayBackground->update(dt);
*/    
    Camera *camera = Manager::camera();
    
    Matrix4 rotationMatrix = camera->rotationMatrix();
    
    m_vPosition[0] = m_fPanX;
    m_vPosition[1] = m_fPanY;
    m_vPosition[2] = -30000.0 * m_fScale;
    
 //   LOGI("scale: %f, %f\n", m_fScale, m_vPosition[2]);
    
    rotationMatrix.translate(m_vPosition);
    
    rotationMatrix.rotate(m_fRotX, 1.0f, 0.0f, 0.0f);
    rotationMatrix.rotate(m_fRotY, 0.0f, 1.0f, 0.0f);
    rotationMatrix.rotate(m_fRotZ, 0.0f, 0.0f, 1.0f);
    
    float scale = m_fScale;
//    if (Manager::renderer()->isHighRez()) {
//        m_pEffect->setRelativeSize(8.0);
//        if (scale < 0.5) {
//            scale = 0.5;
//        }
//    }
//    else {
        m_pEffect->setRelativeSize(3.0);
        
        if (scale < 0.25) {
            scale = 0.25;
        }
//    }
/*
    m_pNebula->setRelativeSize(50.0);
    m_pNebula->pan(m_fPanX, m_fPanY);
    m_pNebula->rotate(m_fRotX, m_fRotY + kPi, m_fRotZ);
    m_pNebula->scale(m_fScale);
    m_pNebula->update(dt);
*/    
    m_pEffect->setScale(scale);
    m_pEffect->setMatrices(camera->projectionMatrixPerspective(), rotationMatrix);
}

void MapScene::render()
{
    Renderer *renderer = Manager::renderer();
    renderer->clearScreen();
    
//    m_pNebula->render();

//    m_pMilkywayBackground->render();
    
    renderer->setBlendMode(BLEND_ALPHA_ONE);
    renderer->enableDepthTest(false);
#ifndef OPENGL_ES
    glEnable(GL_PROGRAM_POINT_SIZE);
#endif
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiTextureID);
    m_pEffect->prepareToRender();
    m_pPointsBatch->renderInterleavedVertexSizeColorIndex();
    
    // Restore render state to normal state
    renderer->setBlendMode(BLEND_ALPHA);
    renderer->enableDepthTest(true);
#ifndef OPENGL_ES
    glDisable(GL_PROGRAM_POINT_SIZE);
#endif    
    
    Manager::shipController()->render();
}

void MapScene::touchesBegan(float x, float y)
{
    if (m_bGesturing) {
        return;
    }
    
    Manager::shipController()->touchesBegan(x, y);
    
    m_vStartPoint[0] = x;
    m_vStartPoint[1] = y;
}

void MapScene::touchesMoved(float x, float y)
{
    if (m_bGesturing) {
        return;
    }

    Manager::shipController()->touchesMoved(x, y);
    
    float xdiff = x - m_vStartPoint[0];
    float ydiff = y - m_vStartPoint[1];
    
    m_fRotX -= (ydiff * .01);
    m_fRotY += (xdiff * .01);
    
    m_vStartPoint[0] = x;
    m_vStartPoint[1] = y;
}

void MapScene::touchesEnded(float x, float y)
{
    if (m_bGesturing) {
        return;
    }

    Manager::shipController()->touchesEnded(x, y);
}

#ifdef WIN32
void MapScene::keyDown(unsigned short keycode)
{
    switch (keycode) {
        case 87: // w - down
            m_bPanDown = true;
            break;
        case 83: // s - up
            m_bPanUp = true;
            break;
        case 65: // a - left
            m_bPanLeft = true;
            break;
        case 68: // d - right
            m_bPanRight = true;
            break;
        case 80: // Move selected;
        {
            int numStars = m_pGalaxyEngine->GetNumStars();
            GalaxyStar *stars = m_pGalaxyEngine->GetStars();
            
            int index = (int)m_pEffect->getSelected();
            Vector3dC cur_pos = stars[index].m_pos;
            
            int indexShortest = 0;
            int indexLongest = 0;
            float distMin = 1000000000.0;
            float distMax = 0.0;
            
            for (int i = 0; i < numStars; i++) {
                if (i != index) {
                    double x_diff = cur_pos.x - stars[i].m_pos.x;
                    double y_diff = cur_pos.y - stars[i].m_pos.y;
                    double z_diff = cur_pos.z - stars[i].m_pos.z;
                    float dist = sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff);
                    
                    if (dist < distMin) {
                        distMin = dist;
                        indexShortest = i;
                    }
                    if (dist > distMax) {
                        distMax = dist;
                        indexLongest = i;
                    }
                }
            }
            
            m_pEffect->setSelected((float)indexShortest);
            LOGI("distShortest: %f, %d\n", distMin * 2.0, indexShortest);
            LOGI("distLongest: %f, %d\n", distMax * 2.0, indexLongest);
            break;
        }
        case 79: // Move selected;
        {
            int numStars = m_pGalaxyEngine->GetNumStars();
            GalaxyStar *stars = m_pGalaxyEngine->GetStars();
            
            int index = (int)m_pEffect->getSelected();
            Vector3dC cur_pos = stars[index].m_pos;
            
            int indexShortest = 0;
            int indexLongest = 0;
            float distMin = 1000000000.0;
            float distMax = 0.0;
            
            for (int i = 0; i < numStars; i++) {
                if (i != index) {
                    double x_diff = cur_pos.x - stars[i].m_pos.x;
                    double y_diff = cur_pos.y - stars[i].m_pos.y;
                    double z_diff = cur_pos.z - stars[i].m_pos.z;
                    float dist = sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff);
                    
                    if (dist < distMin) {
                        distMin = dist;
                        indexShortest = i;
                    }
                    if (dist > distMax) {
                        distMax = dist;
                        indexLongest = i;
                    }
                }
            }
            
            LOGI("distShortest: %f, %d\n", distMin * 2.0, indexShortest);
            LOGI("distLongest: %f, %d\n", distMax * 2.0, indexLongest);
            m_pEffect->setSelected((float)indexLongest);
            break;
        }
        case 73: // i scale up
            m_bScaleUp = true;
            break;
        case 76: // l - rotate right
            m_bRotateRight = true;
            break;
        case 74: // j - rotate left
            m_bRotateLeft = true;
            break;
        case 75: // k - scale down
            m_bScaleDown = true;
            break;
    }

    Manager::shipController()->keyDown(keycode);
}

void MapScene::keyUp(unsigned short keycode)
{
    switch (keycode) {
        case 87: // w - down
            m_bPanDown = false;
            break;
        case 83: // s - up
            m_bPanUp = false;
            break;
        case 65: // a - left
            m_bPanLeft = false;
            break;
        case 68: // d - right
            m_bPanRight = false;
            break;
        case 73: // i scale up
            m_bScaleUp = false;
            break;
        case 76: // l - rotate right
            m_bRotateRight = false;
            break;
        case 74: // j - rotate left
            m_bRotateLeft = false;
            break;
        case 75: // k - scale down
            m_bScaleDown = false;
            break;
    }

    Manager::shipController()->keyUp(keycode);
}
#else
void MapScene::keyDown(unsigned short keycode)
{
    switch (keycode) {
        case 13: // w - down
            m_bPanDown = true;
            break;
        case 1: // s - up
            m_bPanUp = true;
            break;
        case 0: // a - left
            m_bPanLeft = true;
            break;
        case 2: // d - right
            m_bPanRight = true;
            break;
        case 35: // Move selected;
        {
            int numStars = m_pGalaxyEngine->GetNumStars();
            GalaxyStar *stars = m_pGalaxyEngine->GetStars();
            
            int index = (int)m_pEffect->getSelected();
            Vector3dC cur_pos = stars[index].m_pos;
            
            int indexShortest = 0;
            int indexLongest = 0;
            float distMin = 1000000000.0;
            float distMax = 0.0;
            
            for (int i = 0; i < numStars; i++) {
                if (i != index) {
                    double x_diff = cur_pos.x - stars[i].m_pos.x;
                    double y_diff = cur_pos.y - stars[i].m_pos.y;
                    double z_diff = cur_pos.z - stars[i].m_pos.z;
                    float dist = sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff);
                    
                    if (dist < distMin) {
                        distMin = dist;
                        indexShortest = i;
                    }
                    if (dist > distMax) {
                        distMax = dist;
                        indexLongest = i;
                    }
                }
            }
            
            m_pEffect->setSelected((float)indexShortest);
            LOGI("distShortest: %f, %d\n", distMin * 2.0, indexShortest);
            LOGI("distLongest: %f, %d\n", distMax * 2.0, indexLongest);
            break;
        }
        case 31: // Move selected;
        {
            int numStars = m_pGalaxyEngine->GetNumStars();
            GalaxyStar *stars = m_pGalaxyEngine->GetStars();
            
            int index = (int)m_pEffect->getSelected();
            Vector3dC cur_pos = stars[index].m_pos;
            
            int indexShortest = 0;
            int indexLongest = 0;
            float distMin = 1000000000.0;
            float distMax = 0.0;
            
            for (int i = 0; i < numStars; i++) {
                if (i != index) {
                    double x_diff = cur_pos.x - stars[i].m_pos.x;
                    double y_diff = cur_pos.y - stars[i].m_pos.y;
                    double z_diff = cur_pos.z - stars[i].m_pos.z;
                    float dist = sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff);
                    
                    if (dist < distMin) {
                        distMin = dist;
                        indexShortest = i;
                    }
                    if (dist > distMax) {
                        distMax = dist;
                        indexLongest = i;
                    }
                }
            }
            
            LOGI("distShortest: %f, %d\n", distMin * 2.0, indexShortest);
            LOGI("distLongest: %f, %d\n", distMax * 2.0, indexLongest);
            m_pEffect->setSelected((float)indexLongest);
            break;
        }
        case 34: // i scale up
            m_bScaleUp = true;
            break;
        case 37: // l - rotate right
            m_bRotateRight = true;
            break;
        case 38: // j - rotate left
            m_bRotateLeft = true;
            break;
        case 40: // k - scale down
            m_bScaleDown = true;
            break;
    }

    Manager::shipController()->keyDown(keycode);
}

void MapScene::keyUp(unsigned short keycode)
{
    switch (keycode) {
        case 13: // w - down
            m_bPanDown = false;
            break;
        case 1: // s - up
            m_bPanUp = false;
            break;
        case 0: // a - left
            m_bPanLeft = false;
            break;
        case 2: // d - right
            m_bPanRight = false;
            break;
        case 34: // i scale up
            m_bScaleUp = false;
            break;
        case 37: // l - rotate right
            m_bRotateRight = false;
            break;
        case 38: // j - rotate left
            m_bRotateLeft = false;
            break;
        case 40: // k - scale down
            m_bScaleDown = false;
            break;
    }

    Manager::shipController()->keyUp(keycode);
}
#endif

void MapScene::pinchBegan()
{
    m_bGesturing = true;
    
    m_fPinchInitial = 1.0;
}

void MapScene::pinchChanged(float scale)
{
    m_bGesturing = true;

    m_fScale /= scale / m_fPinchInitial;
    
    m_fPinchInitial = scale;
}

void MapScene::pinchEnded()
{
    m_bGesturing = false;
}

void MapScene::panBegan()
{
    m_bGesturing = true;
    
    m_fPanInitialX = 0;
    m_fPanInitialY = 0;
}

void MapScene::panChanged(float x, float y)
{
    m_bGesturing = true;
    
    m_fPanX += (x - m_fPanInitialX) * m_fScale * 30.0;
    m_fPanY += (-(y - m_fPanInitialY)) * m_fScale * 30.0;
    
    m_fPanInitialX = x;
    m_fPanInitialY = y;
}

void MapScene::panEnded()
{
    m_bGesturing = false;
}

void MapScene::rotateBegan()
{
    m_bGesturing = true;
    
    m_fRotInitial = 0.0;
}

void MapScene::rotateChanged(float rotation)
{
    m_bGesturing = true;
    
    m_fRotZ -= rotation - m_fRotInitial;
    
    m_fRotInitial = rotation;
}

void MapScene::rotateEnded()
{
    m_bGesturing = false;
}

Color MapScene::colorFromTemperature(double temp) const
{
    int idx = (temp - m_t0) / (m_t1-m_t0) * m_colNum;
    idx = std::min(m_colNum-1, idx);
    idx = std::max(0, idx);
    return m_col[idx];
}

