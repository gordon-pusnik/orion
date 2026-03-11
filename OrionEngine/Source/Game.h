#ifndef __GAME_H__
#define __GAME_H__

#ifdef __ANDROID__
#include <android/asset_manager.h>
#endif

#ifdef __APPLE__
#import <GLKit/GLKit.h>
#ifdef OPENGL_ES
#import "ViewController.h"
#endif
#endif

class Game
{
public:
	Game();
	~Game();
#ifdef __ANDROID__
    int init(float viewWidth, float viewHeight, AAssetManager *mgr, const char *dir, bool hirez = false);
#endif
    int init(float viewWidth, float viewHeight, bool hirez = false);
    void touchesBegan(float x, float y);
    void touchesMoved(float x, float y);
    void touchesEnded(float x, float y);
    void update(long ms);
    void update(double ms);
    void updateWithDt(double ms);
	void render();
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
    bool multiTouchEnabled();

#ifdef __APPLE__
#ifdef OPENGL_ES
    void setViewController(ViewController* viewController);
#endif
#endif

private:
    double m_dStartTimePhysics;
    double m_dEndTimePhysics;
    double m_dStartTime;
    double m_dEndTime;
};

#endif
