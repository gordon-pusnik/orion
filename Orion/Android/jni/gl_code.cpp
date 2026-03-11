/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include "../../../OrionEngine/Source/Game.h"

Game *_game = 0;
float _height = 0;

bool init(int w, int h, AAssetManager *mgr, const char* dir)
{
	_height = h;

	if (! _game) {
		_game = new Game;
		_game->init(w, h, mgr, dir);
	}

    return true;
}

void destroy()
{
	if (_game) {
		delete _game;
		_game = 0;
	}
}

void touchesBegan(float x, float y)
{
	if (_game) {
		_game->touchesBegan(x, _height - y);
	}
}

void touchesMoved(float x, float y)
{
	if (_game) {
		_game->touchesMoved(x, _height - y);
	}
}

void touchesEnded(float x, float y)
{
	if (_game) {
		_game->touchesEnded(x, _height - y);
	}
}

void pinchBegan()
{
    if (_game) {
        _game->pinchBegan();
    }
}

void pinchChanged(float scale)
{
    if (_game) {
        _game->pinchChanged(scale);
    }
}

void pinchEnded()
{
    if (_game) {
        _game->pinchEnded();
    }
}

void panBegan()
{
	if (_game) {
		_game->panBegan();
	}
}

void panChanged(float x, float y)
{
	if (_game) {
		_game->panChanged(x, y);
	}
}

void panEnded()
{
	if (_game) {
		_game->panEnded();
	}
}

void rotateBegan()
{
	if (_game) {
		_game->rotateBegan();
	}
}

void rotateChanged(float rotation)
{
	if (_game) {
		_game->rotateChanged(rotation);
	}
}

void rotateEnded()
{
	if (_game) {
		_game->rotateEnded();
	}
}

void update(long ms)
{
	if (_game) {
		_game->update(ms);
	}
}

void render()
{
    if (_game) {
    	_game->render();
    }
}

bool gesturesEnabled() {
	bool ret = false;
	if (_game) {
		ret = ! _game->multiTouchEnabled();
	}
	return ret;
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_init(JNIEnv * env, jobject obj, jint width, jint height, jobject mgr, jstring dir);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_touchesBegan(JNIEnv * env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_touchesMoved(JNIEnv * env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_touchesEnded(JNIEnv * env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_pinchBegan(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_pinchChanged(JNIEnv * env, jobject obj, jfloat scale);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_pinchEnded(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_panBegan(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_panChanged(JNIEnv * env, jobject obj, jfloat x, jfloat y);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_panEnded(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_rotateBegan(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_rotateChanged(JNIEnv * env, jobject obj, jfloat rotation);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_rotateEnded(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_update(JNIEnv * env, jobject obj, jlong ms);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_render(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_destroy(JNIEnv * env, jobject obj);
    JNIEXPORT jboolean JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_gesturesEnabled(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_init(JNIEnv * env, jobject obj, jint width, jint height, jobject mgr, jstring dir)
{
    const char* temp = env->GetStringUTFChars(dir, NULL);
	AAssetManager *amgr = AAssetManager_fromJava(env, mgr);
    init(width, height, amgr, temp);
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_touchesBegan(JNIEnv * env, jobject obj, jfloat x, jfloat y)
{
    touchesBegan(x, y);
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_touchesMoved(JNIEnv * env, jobject obj, jfloat x, jfloat y)
{
    touchesMoved(x, y);
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_touchesEnded(JNIEnv * env, jobject obj, jfloat x, jfloat y)
{
    touchesEnded(x, y);
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_pinchBegan(JNIEnv * env, jobject obj)
{
    pinchBegan();
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_pinchChanged(JNIEnv * env, jobject obj, jfloat scale)
{
    pinchChanged(scale);
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_pinchEnded(JNIEnv * env, jobject obj)
{
    pinchEnded();
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_panBegan(JNIEnv * env, jobject obj)
{
    panBegan();
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_panChanged(JNIEnv * env, jobject obj, jfloat x, jfloat y)
{
    panChanged(x, y);
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_panEnded(JNIEnv * env, jobject obj)
{
    panEnded();
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_rotateBegan(JNIEnv * env, jobject obj)
{
    rotateBegan();
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_rotateChanged(JNIEnv * env, jobject obj, jfloat rotation)
{
    rotateChanged(rotation);
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_rotateEnded(JNIEnv * env, jobject obj)
{
    rotateEnded();
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_update(JNIEnv * env, jobject obj, jlong ms)
{
    update(ms);
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_render(JNIEnv * env, jobject obj)
{
    render();
}

JNIEXPORT void JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_destroy(JNIEnv * env, jobject obj)
{
    destroy();
}

JNIEXPORT jboolean JNICALL Java_com_rythmiasoftware_orion_GL2JNILib_gesturesEnabled(JNIEnv * env, jobject obj)
{
	return gesturesEnabled();
}

