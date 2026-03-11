# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

ORION_ENGINE_SOURCE := ../../../OrionEngine/Source

LOCAL_MODULE    := libgl2jni
LOCAL_CFLAGS    := -DOPENGL_ES=1 -D__ANDROID__=1
LOCAL_CPPFLAGS  := -std=c++11
LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                    $(LOCAL_PATH)/$(ORION_ENGINE_SOURCE) \
                    $(LOCAL_PATH)/$(ORION_ENGINE_SOURCE)/png

LOCAL_SRC_FILES := gl_code.cpp $(ORION_ENGINE_SOURCE)/Camera.cpp \
$(ORION_ENGINE_SOURCE)/Game.cpp $(ORION_ENGINE_SOURCE)/GLBatch.cpp $(ORION_ENGINE_SOURCE)/GLBaseEffect.cpp \
$(ORION_ENGINE_SOURCE)/GLTexturePointLightDiffuseEffect.cpp $(ORION_ENGINE_SOURCE)/GLUtils.cpp \
$(ORION_ENGINE_SOURCE)/Matrix4.cpp $(ORION_ENGINE_SOURCE)/Planet.cpp $(ORION_ENGINE_SOURCE)/Quaternion.cpp \
$(ORION_ENGINE_SOURCE)/Renderer.cpp $(ORION_ENGINE_SOURCE)/ShaderUtils.cpp $(ORION_ENGINE_SOURCE)/Skybox.cpp \
$(ORION_ENGINE_SOURCE)/Scene.cpp $(ORION_ENGINE_SOURCE)/Utils.cpp $(ORION_ENGINE_SOURCE)/Vector2.cpp \
$(ORION_ENGINE_SOURCE)/Vector3.cpp $(ORION_ENGINE_SOURCE)/Vector4.cpp $(ORION_ENGINE_SOURCE)/png/adler32.c \
$(ORION_ENGINE_SOURCE)/png/crc32.c $(ORION_ENGINE_SOURCE)/png/infblock.c $(ORION_ENGINE_SOURCE)/png/infcodes.c \
$(ORION_ENGINE_SOURCE)/png/inffast.c $(ORION_ENGINE_SOURCE)/png/inflate.c $(ORION_ENGINE_SOURCE)/png/inftrees.c \
$(ORION_ENGINE_SOURCE)/png/infutil.c $(ORION_ENGINE_SOURCE)/png/png.c $(ORION_ENGINE_SOURCE)/png/pngerror.c \
$(ORION_ENGINE_SOURCE)/png/pnggccrd.c $(ORION_ENGINE_SOURCE)/png/pngget.c $(ORION_ENGINE_SOURCE)/png/PngLoader.cpp \
$(ORION_ENGINE_SOURCE)/png/pngmem.c $(ORION_ENGINE_SOURCE)/png/pngread.c $(ORION_ENGINE_SOURCE)/png/pngrio.c \
$(ORION_ENGINE_SOURCE)/png/pngrtran.c $(ORION_ENGINE_SOURCE)/png/pngrutil.c $(ORION_ENGINE_SOURCE)/png/pngset.c \
$(ORION_ENGINE_SOURCE)/png/pngtrans.c $(ORION_ENGINE_SOURCE)/png/pngvcrd.c $(ORION_ENGINE_SOURCE)/png/zutil.c \
$(ORION_ENGINE_SOURCE)/Texture.cpp $(ORION_ENGINE_SOURCE)/AssetManager.cpp $(ORION_ENGINE_SOURCE)/GLCubemapEffect.cpp \
$(ORION_ENGINE_SOURCE)/PitchYawController.cpp $(ORION_ENGINE_SOURCE)/GLTextureEffect.cpp $(ORION_ENGINE_SOURCE)/Star.cpp \
$(ORION_ENGINE_SOURCE)/FwdRevController.cpp $(ORION_ENGINE_SOURCE)/RollController.cpp $(ORION_ENGINE_SOURCE)/SceneManager.cpp \
$(ORION_ENGINE_SOURCE)/ShipController.cpp $(ORION_ENGINE_SOURCE)/MultitexPlanet.cpp $(ORION_ENGINE_SOURCE)/GLCoronaEffect.cpp \
$(ORION_ENGINE_SOURCE)/GLMultiTexturePlanetEffect.cpp $(ORION_ENGINE_SOURCE)/Manager.cpp $(ORION_ENGINE_SOURCE)/GeometryManager.cpp \
$(ORION_ENGINE_SOURCE)/Starfield.cpp $(ORION_ENGINE_SOURCE)/GLPointSpriteStarEffect.cpp $(ORION_ENGINE_SOURCE)/ShaderManager.cpp \
$(ORION_ENGINE_SOURCE)/Button.cpp $(ORION_ENGINE_SOURCE)/Imposter.cpp $(ORION_ENGINE_SOURCE)/Nebula.cpp $(ORION_ENGINE_SOURCE)/GLNebulaEffect.cpp \
$(ORION_ENGINE_SOURCE)/TimeManager.cpp $(ORION_ENGINE_SOURCE)/SpaceScene.cpp $(ORION_ENGINE_SOURCE)/MapScene.cpp $(ORION_ENGINE_SOURCE)/SlidePanel.cpp \
$(ORION_ENGINE_SOURCE)/GLPointSpriteStarEffect2.cpp $(ORION_ENGINE_SOURCE)/GLParticleEffect.cpp $(ORION_ENGINE_SOURCE)/GLFrameBuffer.cpp \
$(ORION_ENGINE_SOURCE)/CumulativeDistributionFunction.cpp $(ORION_ENGINE_SOURCE)/FastMath.cpp $(ORION_ENGINE_SOURCE)/GalaxyEngine.cpp \
$(ORION_ENGINE_SOURCE)/SpecRend.cpp $(ORION_ENGINE_SOURCE)/ObjLoader.cpp $(ORION_ENGINE_SOURCE)/Ship.cpp $(ORION_ENGINE_SOURCE)/ModelOBJ.cpp $(ORION_ENGINE_SOURCE)/Ghoul.cpp \
$(ORION_ENGINE_SOURCE)/CobraFighter.cpp $(ORION_ENGINE_SOURCE)/Engine.cpp $(ORION_ENGINE_SOURCE)/Eris.cpp $(ORION_ENGINE_SOURCE)/Station.cpp $(ORION_ENGINE_SOURCE)/Kono.cpp \
$(ORION_ENGINE_SOURCE)/CobraInterceptor.cpp $(ORION_ENGINE_SOURCE)/AnnaV.cpp $(ORION_ENGINE_SOURCE)/KanaraInterceptor.cpp $(ORION_ENGINE_SOURCE)/TextureManager.cpp \
$(ORION_ENGINE_SOURCE)/GLPointSpriteReticleEffect.cpp $(ORION_ENGINE_SOURCE)/GLPointSpriteEngineEffect.cpp $(ORION_ENGINE_SOURCE)/Random.cpp \
$(ORION_ENGINE_SOURCE)/Hud.cpp $(ORION_ENGINE_SOURCE)/Reticle.cpp

LOCAL_LDLIBS    := -llog -lGLESv2 -landroid

include $(BUILD_SHARED_LIBRARY)
