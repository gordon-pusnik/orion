//
//  Skybox.cpp
//  Orion
//
//  Created by Gordon Pusnik on 2012-11-01.
//  Copyright (c) 2012 Rythmia Software. All rights reserved.
//

#include "Skybox.h"
#include "GLUtils.h"
#include "Texture.h"
#include "Manager.h"
#include "Defines.h"
#include "Utils.h"
#include "Starfield.h"
#include "Manager.h"
#include "Star.h"
#include "Constants.h"
#include "Imposter.h"
#include "Nebula.h"

#define TEX_SIZE 1024

// Six sides of a cube map
const char *szCubeFaces[6] = { "Images/red_nebula_cubemap_right1.png",
                               "Images/red_nebula_cubemap_left2.png",
                               "Images/red_nebula_cubemap_top3.png",
                               "Images/red_nebula_cubemap_bottom4.png",
                               "Images/red_nebula_cubemap_front5.png",
                               "Images/red_nebula_cubemap_back6.png" };

GLenum  cube[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                     GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                     GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

Skybox::Skybox()
{
    
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &m_uiTextureID);
}

void Skybox::init(Vector3f position, int scale)
{
    m_vPosition[0] = position[0];
    m_vPosition[1] = position[1];
    m_vPosition[2] = position[2];

    if (1) {
        genStars();
//        genNebula();
        initTextures();
    }
    else {    
        // Load texture.
        glGenTextures(1, &m_uiTextureID);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTextureID);
        
        // Set up texture maps
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Load Cube Map images
        GLbyte *pBits;
        int nWidth, nHeight, nComponents;
        GLenum eFormat;
        for (int i = 0; i < 6; i++) {
            // Load this texture map
            Texture *tex = new Texture(szCubeFaces[i]);
            pBits = (GLbyte*)tex->mVBitMap->pixels;
            nWidth = tex->mVBitMap->width;
            nHeight = tex->mVBitMap->height;
            nComponents = GL_RGBA;
            eFormat = GL_RGBA;

            glTexImage2D(cube[i], 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);
            
            delete tex;
        }
    }
    
    // Check if this shader has already been created, otherwise create it.
    ShaderManager *shaderManager = Manager::shaderManager();
    Shader *shader = shaderManager->getShaderForEffectType(CUBEMAP_EFFECT);
    if (shader) {
        m_pEffect = (GLCubemapEffect*) shader->m_pEffect;
    }
    else {
        m_pEffect = new GLCubemapEffect;
        m_pEffect->init();
        shaderManager->setShaderForEffectType(CUBEMAP_EFFECT, m_pEffect);
    }
    
    int numVertices;
#if INTERLEAVE_DATA
    m_iNumIndices = GLUtils::genCube(scale, &m_pVertices, &m_pIndices, &numVertices);
#else
    m_iNumIndices = GLUtils::genCube(scale, &m_pVertices, &m_pNormals, &m_pTexCoords, &m_pIndices, &numVertices);
#endif
    
#if USE_VBOS
#if INTERLEAVE_DATA
    m_triangleBatch.beginInterleaved(GL_TRIANGLES, numVertices, 0);
    m_triangleBatch.copyIndices(m_pIndices, m_iNumIndices);
    m_triangleBatch.copyInterleavedVertexData(m_pVertices);
    m_triangleBatch.endInterleaved();
    
    delete[] m_pVertices;
    delete[] m_pIndices;
#else
    m_triangleBatch.begin(GL_TRIANGLES, numVertices, 1);
    m_triangleBatch.copyIndices(m_pIndices, m_iNumIndices);
    m_triangleBatch.copyVertexData3f(m_pVertices);
    m_triangleBatch.copyNormalData3f(m_pNormals);
    m_triangleBatch.copyTexCoordData2f(m_pTexCoords, 0);
    m_triangleBatch.end();
    
    delete[] m_pVertices;
    delete[] m_pNormals;
    delete[] m_pTexCoords;
    delete[] m_pIndices;
#endif
#endif    
}

void Skybox::initTextures()
{
/*
    // Load texture.
    glGenTextures(1, &m_uiTextureID);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTextureID);
    
    // Set up texture maps
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    for (int i = 0; i < 6; i++) {
        // Reserve this texture
        glTexImage2D(cube[i], 0, GL_RGBA, TEX_SIZE, TEX_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
*/

    // Load texture.
    glGenTextures(1, &m_uiTextureID);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTextureID);
    
    // Set up texture maps
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    // Load Cube Map images
    GLbyte *pBits;
    int nWidth, nHeight, nComponents;
    GLenum eFormat;
    for (int i = 0; i < 6; i++) {
        // Load this texture map
        Texture *tex = new Texture(szCubeFaces[i]);
        pBits = (GLbyte*)tex->mVBitMap->pixels;
        nWidth = tex->mVBitMap->width;
        nHeight = tex->mVBitMap->height;
        nComponents = GL_RGBA;
        eFormat = GL_RGBA;
        
        glTexImage2D(cube[i], 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);
        
        delete tex;
    }
    
    // Create and bind FBO
    glGenFramebuffers(1, &m_uiFboID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiFboID);
    
    // Attach face to fbo;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_uiTextureID, 0);
/*
    // Create depth renderbuffer
    glGenRenderbuffers(1, &m_uiDepthBufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthBufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, TEX_SIZE, TEX_SIZE);
    
    // Attach depth buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_uiDepthBufferID);
*/
    //Does the GPU support current FBO configuration?
    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    
    if (status == GL_FRAMEBUFFER_COMPLETE) {
        // Make a new camera and save the old one.
        Camera *oldCamera = Manager::camera();
        Vector3f pos = { 0.0, 0.0, 0.0 };
        Camera *camera = new Camera();
        camera->init(pos, TEX_SIZE, TEX_SIZE);
        Manager::m_sCamera = camera;
        int w = Manager::renderer()->getViewPortWidth();
        int h = Manager::renderer()->getViewPortHeight();
        Manager::renderer()->setViewport(0, 0, TEX_SIZE, TEX_SIZE);
        
        // Render to the textures;
        for (int i = 0; i < 6; i++) {
            switch(i) {
                case 0: // Front
                    // Attach to the face.
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_uiTextureID, 0);
                    Manager::camera()->move(0, 0, 0);
                    break;
                case 1: // Right
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_uiTextureID, 0);
                    Manager::camera()->move(7.8616, 0, 0);
                    break;
                case 2: // Back
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_uiTextureID, 0);
                    Manager::camera()->move(7.8616, 0, 0);
                    break;
                case 3: // Left
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_uiTextureID, 0);
                    Manager::camera()->move(7.8616, 0, 0);
                    break;
                case 4: // Up
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_uiTextureID, 0);
                    Manager::camera()->move(7.8616, 0, 0);
                    Manager::camera()->move(0, 7.8616, 0);
                    break;
                case 5: // Down
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_uiTextureID, 0);
                    Manager::camera()->move(0, 7.8616, 0);
                    Manager::camera()->move(0, 7.8616, 0);
                    break;
            };
            
            Manager::camera()->update(0);
            
//            Manager::renderer()->clearScreen();
            
//            m_pNebula->update(1);
//            m_pNebula->render();
            
            m_pStarfield->update(1);
            m_pStarfield->render();
            
//            m_pStar->update(1);
//            m_pStar->render();
        }
        
        Manager::renderer()->setViewport(0, 0, w, h);
        Manager::m_sCamera = oldCamera;
        delete camera;
    }

    delete m_pStarfield;
//    delete m_pStar;
//    delete m_pNebula;

    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    glDeleteFramebuffers(1, &m_uiFboID);
//    glDeleteRenderbuffers(1, &m_uiDepthBufferID);
}

void Skybox::genStars()
{
    m_pStarfield = new Starfield;
    m_pStarfield->init();

//    Vector3f pos = {20.0, 0.0, 200.0};
//    m_pStar = new Imposter;
//    m_pStar->init(pos, 200.0, "Images/Star.png", BLEND_ADDITIVE, true, true);
}

void Skybox::genNebula()
{
    m_pNebula = new Nebula;
    Vector3f pos = {0.0, 0.0, -40.0};
    Vector4f color = {0.010, 0.007, 0.020, 1.0};
    m_pNebula->init(pos, 90, 35.0, 45.0, 500.0, color, BLEND_ALPHA_ONE, "Images/smoke.png");
}

void Skybox::update(float dt)
{
    Camera *camera = Manager::camera();
    m_rotationMatrix = camera->rotationMatrix();
    m_rotationMatrix.translate(m_vPosition);
}

void Skybox::render()
{
    Renderer *renderer = Manager::renderer();
    renderer->enableDepthTest(false);
    renderer->enableCullFace(false);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiTextureID);

    m_pEffect->setMatrices(Manager::camera()->projectionMatrixPerspective(), m_rotationMatrix);
    m_pEffect->prepareToRender();
    
#if USE_VBOS
#if INTERLEAVE_DATA
    m_triangleBatch.renderInterleaved();
#else
    m_triangleBatch.render();
#endif
#else
    glVertexAttribPointer(VertexAttribVertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), m_pVertices);
    glEnableVertexAttribArray(VertexAttribVertex);
    
    glVertexAttribPointer(VertexAttribNormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), m_pNormals);
    glEnableVertexAttribArray(VertexAttribNormal);
    
    glVertexAttribPointer(VertexAttribTexCoord0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), m_pTexCoords);
    glEnableVertexAttribArray(VertexAttribTexCoord0);
    
    glDrawElements(GL_TRIANGLES, m_iNumIndices, GL_UNSIGNED_SHORT, m_pIndices);
    
    glDisableVertexAttribArray(VertexAttribVertex);
    
    glDisableVertexAttribArray(VertexAttribNormal);
    
    glDisableVertexAttribArray(VertexAttribTexCoord0);
#endif
    renderer->enableDepthTest(true);
    renderer->enableCullFace(true);
}
