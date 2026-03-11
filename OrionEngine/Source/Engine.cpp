//
//  Engine.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2013-02-22.
//  Copyright (c) 2013 Rythmia Software. All rights reserved.
//

#include "Engine.h"
#include "Manager.h"
#include "Utils.h"

#ifdef __APPLE__
#ifndef OPENGL_ES
#include <OpenGL/gl.h>
#endif
#endif

Engine::Engine()
{
    m_fPower = 0.0;
}

Engine::~Engine()
{
    Manager::geometryManager()->releaseGeometryForEntityType(ENGINE);
    Manager::shaderManager()->releaseShaderForEffectType(POINT_SPRITE_ENGINE_EFFECT);
    Manager::textureManager()->releaseTextureForFileName("Images/halo.png");
}

void Engine::init(Vector3f pos, Vector4f color)
{
    m_vPosition[0] = pos[0];
    m_vPosition[1] = pos[1];
    m_vPosition[2] = pos[2];

    ShaderManager *shaderManager = Manager::shaderManager();
    Shader *shader = shaderManager->getShaderForEffectType(POINT_SPRITE_ENGINE_EFFECT);
    if (shader) {
        m_pEffect = (GLPointSpriteEngineEffect*) shader->m_pEffect;
    }
    else {
        m_pEffect = new GLPointSpriteEngineEffect;
        m_pEffect->init();
        m_pEffect->setColor(color);
        shaderManager->setShaderForEffectType(POINT_SPRITE_ENGINE_EFFECT, m_pEffect);
    }
    
    GeometryManager *geoManager = Manager::geometryManager();
    Geometry* geometry = geoManager->getGeometryForEntityType(ENGINE);
    if (geometry) {
        m_pPointBatch = geometry->m_pBatch1;
    }
    else {
        m_pPointBatch = new GLBatch;
        geoManager->setGeometryForEntityType(ENGINE,
                                             m_pPointBatch,
                                             0,
                                             0);
        
        GLfloat verts[] = { 0.0, 0.0, 0.0 };
        m_pPointBatch->begin(GL_POINTS, 1, 0);
        m_pPointBatch->copyVertexData3f(verts);
        m_pPointBatch->end();
    }
    
#ifndef OPENGL_ES
    glEnable(GL_POINT_SPRITE);
#endif
    
    TextureManager *textureManager = Manager::textureManager();
    Tex *tex = textureManager->getTextureForFileName("Images/halo.png");
    if (tex) {
        m_uiPointTextureID = tex->m_uiTex;
    }
    else {
        glGenTextures(1, &m_uiPointTextureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_uiPointTextureID);
        
        if (GLUtils::loadPNGTexture("Images/halo.png", GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE)) {
            LOGI("Halo texture loaded\n");
        }
        
        textureManager->setTextureForFileName("Images/halo.png", m_uiPointTextureID);
    }
}

void Engine::update(float dt)
{
    m_pointModelViewMatrix = Matrix4::makeIdentityMatrix();
    
//    Vector3d campos;
//    camera->getPosition(campos);
    
    ShipController *controller = Manager::shipController();
    float pitch = controller->getPitch();
    float yaw = -controller->getYaw();
    float roll = controller->getRoll();
    
    m_vRelPosition[0] = m_vPosition[0] + Utils::getScaled(yaw) * 1.5;
    m_vRelPosition[1] = m_vPosition[1] + Utils::getScaled(pitch) * 2.0;
    m_vRelPosition[2] = m_vPosition[2];
    
    m_vRelPosition[1] += Utils::getScaled(roll) * (m_vPosition[0] * 50000.0);
    
    m_pointModelViewMatrix.translate(m_vRelPosition);
    
//    m_pointModelViewMatrix.rotate(pitch * .005, 1.0f, 0.0f, 0.0f);
//    m_pointModelViewMatrix.rotate(roll * .005, 0.0f, 1.0f, 0.0f);
//    m_pointModelViewMatrix.rotate(yaw * .005, 0.0f, 0.0f, 1.0f);
}

void Engine::render()
{
    Renderer *renderer = Manager::renderer();
    renderer->setBlendMode(BLEND_ALPHA_ONE);
    renderer->enableDepthTest(false);
    //        renderer->enableCullFace(false);
#ifndef OPENGL_ES
    glEnable(GL_PROGRAM_POINT_SIZE);
#endif
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_uiPointTextureID);
    
    m_pEffect->setPower(m_fPower);
    m_pEffect->setMatrices(Manager::camera()->projectionMatrixPerspective(), m_pointModelViewMatrix);
    m_pEffect->prepareToRender();
    
    m_pPointBatch->renderInterleavedColor();
    
    // Restore render state to normal state
    renderer->setBlendMode(BLEND_ALPHA);
    renderer->enableDepthTest(true);
    //        renderer->enableCullFace(true);
#ifndef OPENGL_ES
    glDisable(GL_PROGRAM_POINT_SIZE);
#endif
    
}
