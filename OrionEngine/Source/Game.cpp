#include "Game.h"
#include "Utils.h"
#include "Manager.h"
#include <cstdlib>
#include "Random.h"

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#elif WIN32
#include "stdafx.h"
#include <GL/glew.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

Game::Game()
{
    LOGI("Game created\n");
}

Game::~Game()
{
    delete Manager::m_sRenderer;
    delete Manager::m_sSceneManager;
    delete Manager::m_sAssetManager;
    delete Manager::m_sGeometryManager;
    delete Manager::m_sShaderManager;
    delete Manager::m_sCamera;
    
    LOGI("Game destroyed\n");
}

#ifdef __APPLE__
#ifdef OPENGL_ES
void Game::setViewController(ViewController* viewController)
{
    Manager::sceneManager()->setViewController(viewController);
}
#endif
#endif

#ifdef __ANDROID__
int Game::init(float viewWidth, float viewHeight, AAssetManager *mgr, const char *dir, bool hirez)
{
	LOGI("Files dir: %s\n", dir);

    Manager::m_sAssetManager = new AssetManager;
    Manager::assetManager()->setAAssetManager(mgr);
    Manager::assetManager()->setFilesDir(dir);
    
    return init(viewWidth, viewHeight, hirez);
}
#endif

int Game::init(float viewWidth, float viewHeight, bool hirez)
{
    LOGI("Game::init()\n");
    
    Random::seedRand(123456789);
    
#ifndef __ANDROID__
    Manager::m_sAssetManager = new AssetManager;
#endif
    Manager::m_sRenderer = new Renderer;
    Manager::renderer()->init(hirez);
    Manager::renderer()->enableDepthTest(true);
    Manager::renderer()->enableCullFace(true);
    Manager::renderer()->setBlendMode(BLEND_ALPHA);
    Manager::renderer()->setViewport(0, 0, viewWidth, viewHeight);
    Manager::renderer()->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    Vector3f v = {0.0, 0.0, 0.0};
    Manager::m_sCamera = new Camera;
    Manager::camera()->init(v, viewWidth, viewHeight);
    
    Manager::m_sGeometryManager = new GeometryManager;
    
    Manager::m_sShaderManager = new ShaderManager;
    
    Manager::m_sTextureManager = new TextureManager;
    
    Manager::m_sTimeManager = new TimeManager;
    
    Manager::m_sSceneManager = new SceneManager;
    Manager::sceneManager()->init(viewWidth, viewHeight);
    
    m_dStartTime = 0;
    m_dEndTime = 0;
    m_dStartTimePhysics = 0;
    m_dEndTimePhysics = 0;
    
    LOGI("Game inited\n");
    
    return 0;
}

void Game::update(long ms)
{
    m_dEndTime = ms;
    if (m_dStartTime != 0) {
        double dt = (m_dEndTime - m_dStartTime) / 1000.0;
        updateWithDt(dt);
    }
    m_dStartTime = m_dEndTime;
}

void Game::update(double ms)
{
    m_dEndTime = ms;
    if (m_dStartTime != 0) {
        double dt = m_dEndTime - m_dStartTime;
        updateWithDt(dt);
    }
    m_dStartTime = m_dEndTime;
}

void Game::updateWithDt(double dt)
{    
//    dt += 1.0;
    Manager::timeManager()->update(dt);
    Manager::sceneManager()->update(dt);
}

void Game::render()
{
    Manager::sceneManager()->render();
}

void Game::keyDown(unsigned short keycode)
{
//    LOGI("keyDown: %d\n", keycode);
    Manager::sceneManager()->keyDown(keycode);
}

void Game::keyUp(unsigned short keycode)
{
//    LOGI("keyUp: %d\n", keycode);
    Manager::sceneManager()->keyUp(keycode);
}

void Game::touchesBegan(float x, float y)
{
//    LOGI("touchesBegan - x: %f, y: %f\n", x, y);
    Manager::sceneManager()->touchesBegan(x, y);
}

void Game::touchesMoved(float x, float y)
{
//    LOGI("touchesMoved - x: %f, y: %f\n", x, y);
    Manager::sceneManager()->touchesMoved(x, y);
}

void Game::touchesEnded(float x, float y)
{
//    LOGI("touchesEnded - x: %f, y: %f\n", x, y);
    Manager::sceneManager()->touchesEnded(x, y);
}

void Game::pinchBegan()
{
    Manager::sceneManager()->pinchBegan();
}

void Game::pinchChanged(float scale)
{
    Manager::sceneManager()->pinchChanged(scale);
}

void Game::pinchEnded()
{
    Manager::sceneManager()->pinchEnded();
}

void Game::panBegan()
{
    Manager::sceneManager()->panBegan();
}

void Game::panChanged(float x, float y)
{
    Manager::sceneManager()->panChanged(x, y);
}

void Game::panEnded()
{
    Manager::sceneManager()->panEnded();
}

void Game::rotateBegan()
{
    Manager::sceneManager()->rotateBegan();
}

void Game::rotateChanged(float rotation)
{
    Manager::sceneManager()->rotateChanged(rotation);
}

void Game::rotateEnded()
{
    Manager::sceneManager()->rotateEnded();
}

bool Game::multiTouchEnabled()
{
    return Manager::sceneManager()->multiTouchEnabled();
}

